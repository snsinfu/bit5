/**
 * SCRAM authentication client.
 *
 * @see {@link https://tools.ietf.org/html/rfc5802}
 *
 * @module
 */

import * as base64 from "https://deno.land/std@0.91.0/encoding/base64.ts";
import * as sha1 from "https://deno.land/std@0.91.0/hash/sha1.ts";

type ClientState =
  | "init"
  | "client-challenge"
  | "server-challenge"
  | "client-response"
  | "server-response"
  | "failure";

const defaultNonceSize = 8;

/** Client composes and verifies SCRAM authentication messages. */
export class Client {
  private username: string;
  private password: string;
  private keys?: Keys;
  private nonce: string;
  private authMessage: string;
  private state: ClientState;

  /** Constructor sets credentials and parameters used in an authentication. */
  constructor(username: string, password: string, nonce?: string) {
    this.username = username;
    this.password = password;
    this.nonce = nonce ?? generateNonce(defaultNonceSize);
    this.authMessage = "";
    this.state = "init";
  }

  /** Composes client-initial-message. */
  composeChallenge(): string {
    if (this.state !== "init") {
      throw new Error("unexpected state");
    }
    this.state = "client-challenge";

    // No channel binding.
    const header = "n,,";

    const challenge = `n=${normalize(this.username)},r=${this.nonce}`;
    this.authMessage += challenge;

    return header + challenge;
  }

  /** Processes server-initial-message. */
  receiveChallenge(challenge: string) {
    if (this.state !== "client-challenge") {
      throw new Error("unexpected state");
    }
    this.state = "server-challenge";

    const attrs = parseAttributes(challenge);

    // Verify server nonce.
    const nonce = attrs.r;

    if (!attrs.r?.startsWith(this.nonce)) {
      throw new AuthError(Reason.BadServerNonce);
    }

    // Verify salt and rounds.
    let salt: Uint8Array | undefined;
    let rounds: number | undefined;

    try {
      salt = base64.decode(attrs.s);
    } catch {
      throw new AuthError(Reason.BadRounds);
    }

    try {
      rounds = parseInt(attrs.i);
    } catch {
      throw new AuthError(Reason.BadRounds);
    }

    if (rounds <= 0) {
      throw new AuthError(Reason.BadRounds);
    }

    this.keys = deriveKeys(this.password, salt, rounds);
    this.nonce = nonce;

    this.authMessage += "," + challenge;
  }

  /** Composes client-final-message. */
  composeResponse(): string {
    if (this.state !== "server-challenge") {
      throw new Error("unexpected state");
    }
    this.state = "client-response";

    // "biws" is the base-64 encoded gs2-header "n,,".
    const responseWithoutProof = `c=biws,r=${this.nonce}`;
    this.authMessage += "," + responseWithoutProof;

    const proof = base64.encode(
      computeProof(
        computeSignature(this.authMessage, this.keys!.stored),
        this.keys!.client,
      ),
    );

    return `${responseWithoutProof},p=${proof}`;
  }

  /** Processes server-final-message. */
  receiveResponse(response: string) {
    if (this.state !== "client-response") {
      throw new Error("unexpected state");
    }
    this.state = "server-response";

    const attrs = parseAttributes(response);

    if (attrs.e) {
      throw new AuthError(Reason.Rejected, attrs.e);
    }

    const verifier = base64.encode(
      computeSignature(this.authMessage, this.keys!.server),
    );

    if (attrs.v !== verifier) {
      throw new AuthError(Reason.BadVerifier);
    }
  }
}

/** Generates a random nonce string. */
function generateNonce(size: number): string {
  return base64.encode(crypto.getRandomValues(new Uint8Array(size)));
}

/** Parses attributes out of a SCRAM message. */
function parseAttributes(str: string): Record<string, string> {
  const attrs: Record<string, string> = {};

  for (const entry of str.split(",")) {
    const pos = entry.indexOf("=");
    if (pos < 1) {
      throw new AuthError(Reason.BadMessage);
    }

    const key = entry.substr(0, pos);
    const value = entry.substr(pos + 1);
    attrs[key] = value;
  }

  return attrs;
}

/** Reason of authentication failure. */
export enum Reason {
  BadMessage = "bad-message",
  BadServerNonce = "bad-server-nonce",
  BadSalt = "bad-salt",
  BadRounds = "bad-rounds",
  BadVerifier = "bad-verifier",
  Rejected = "rejected",
}

/** Error thrown on authentication failure. */
export class AuthError extends Error {
  constructor(public reason: Reason, message?: string) {
    super(message);
  }
}

/** HMAC-derived binary key. */
type Key = Uint8Array;

/** Binary digest. */
type Digest = Uint8Array;

/** Collection of SCRAM authentication keys derived from a plaintext password. */
interface Keys {
  server: Key;
  client: Key;
  stored: Key;
}

/** Derives authentication keys from a plaintext password. */
function deriveKeys(password: string, salt: Uint8Array, rounds: number): Keys {
  const ikm = bytes(normalize(password));
  const key = pbkdf2((msg: Uint8Array) => sign(msg, ikm), salt, rounds, 1);
  const server = sign(bytes("Server Key"), key);
  const client = sign(bytes("Client Key"), key);
  const stored = digest(client);
  return { server, client, stored };
}

/** Computes SCRAM signature. */
function computeSignature(message: string, key: Key): Digest {
  return sign(bytes(message), key);
}

/** Computes SCRAM proof. */
function computeProof(signature: Digest, key: Key): Digest {
  const proof = new Uint8Array(signature.length);
  for (let i = 0; i < proof.length; i++) {
    proof[i] = signature[i] ^ key[i];
  }
  return proof;
}

/** Returns UTF-8 bytes encoding given text. */
function bytes(text: string): Uint8Array {
  return new TextEncoder().encode(text);
}

/** Normalizes password per SASLprep. */
function normalize(password: string): string {
  const unsafe = /[^\x21-\x7e]/;
  if (unsafe.test(password)) {
    throw new Error("saslprep normalization is not implemented");
  }
  return password;
}

/** Computes message digest. */
function digest(msg: Uint8Array): Digest {
  const hash = new sha1.Sha1();
  hash.update(msg);
  return new Uint8Array(hash.arrayBuffer());
}

/** Computes HMAC of a message using given key. */
function sign(msg: Uint8Array, key: Key): Digest {
  const hmac = new sha1.HmacSha1(key);
  hmac.update(msg);
  return new Uint8Array(hmac.arrayBuffer());
}

/** Computes a PBKDF2 key block. */
function pbkdf2(
  prf: (_: Uint8Array) => Digest,
  salt: Uint8Array,
  rounds: number,
  index: number,
): Key {
  let block = new Uint8Array(salt.length + 4);
  block.set(salt);
  block[salt.length + 0] = (index >> 24) & 0xFF;
  block[salt.length + 1] = (index >> 16) & 0xFF;
  block[salt.length + 2] = (index >> 8) & 0xFF;
  block[salt.length + 3] = index & 0xFF;
  block = prf(block);

  const key = block;
  for (let r = 1; r < rounds; r++) {
    block = prf(block);
    for (let i = 0; i < key.length; i++) {
      key[i] ^= block[i];
    }
  }
  return key;
}
