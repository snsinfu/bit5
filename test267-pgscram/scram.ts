import * as base64 from "https://deno.land/std@0.91.0/encoding/base64.ts";
import * as sha256 from "https://deno.land/std@0.91.0/hash/sha256.ts";

export class ProtocolClient {
  private username: string;
  private password: string;
  private nonce: string;

  constructor(username: string, password: string, nonce?: string) {
    this.username = username;
    this.password = password;
    this.nonce = nonce ?? generateNonce(8);
  }

  makeInitialMessage(): string {
    return "n,," + this.makeBareInitialMessage();
  }

  makeBareInitialMessage(): string {
    return `n=${this.username},r=${this.nonce}`;
  }

  makeFinalMessage(challenge: string): string {
    const params = parseMessage(challenge);

    if (!params.r?.startsWith(this.nonce)) {
      throw new Error("invalid nonce");
    }

    const salt = base64.decode(params.s);
    const rounds = parseInt(params.i);
    const client = new AuthClient(this.password, salt, rounds);

    const prefix = this.makeFinalMessageWithoutProof(params.r);
    const authMessage = this.makeBareInitialMessage() +
      "," + challenge + "," + prefix;
    const proof = client.prove(bytes(authMessage));

    return prefix + ",p=" + base64.encode(proof);
  }

  makeFinalMessageWithoutProof(serverNonce: string): string {
    // "biws" is the base-64 of "n,," in the initial message.
    return `c=biws,n=${serverNonce}`;
  }
}

/** Parses SCRAM message as key-value pairs. */
function parseMessage(msg: string): Record<string, string> {
  const params: Record<string, string> = {};

  for (const pair of msg.split(/,/g)) {
    const [key, value] = pair.split("=");
    params[key] = value;
  }

  return params;
}

/** Generates a random string. */
function generateNonce(size: number): string {
  return base64.encode(crypto.getRandomValues(new Uint8Array(size)));
}

/**
 * FIXME: Client should be able to verify a server signature.
 */
class AuthClient {
  private clientKey: Uint8Array;
  private storedKey: Uint8Array;

  constructor(password: string, salt: Uint8Array, rounds: number) {
    const key = deriveKey(bytes(normalize(password)), salt, rounds);
    this.clientKey = sign(bytes("Client key"), key);
    this.storedKey = digest(this.clientKey);
  }

  prove(msg: Uint8Array): Uint8Array {
    const signature = sign(msg, this.storedKey);
    const proof = new Uint8Array(signature.length);
    for (let i = 0; i < proof.length; i++) {
      proof[i] = signature[i] ^ this.clientKey[i];
    }
    return proof;
  }
}

/** Returns UTF-8 encoded string. */
function bytes(text: string): Uint8Array {
  return new TextEncoder().encode(text);
}

/** Normalizes password per saslprep. */
function normalize(password: string): string {
  // Not implemented.
  return password;
}

/**
 * Derives a salted password used to generate client and server keys for SCRAM
 * authentication.
 */
function deriveKey(
  password: Uint8Array,
  salt: Uint8Array,
  rounds: number,
): Uint8Array {
  return pbkdf2((msg: Uint8Array) => sign(msg, password), salt, rounds, 1);
}

/** Computes SHA-256 hash of a message. */
function digest(msg: Uint8Array): Uint8Array {
  const hash = new sha256.Sha256();
  hash.update(msg);
  return new Uint8Array(hash.arrayBuffer());
}

/** Computes HMAC-SHA-256 of a message using given key. */
function sign(msg: Uint8Array, key: Uint8Array): Uint8Array {
  const hmac = new sha256.HmacSha256(key);
  hmac.update(msg);
  return new Uint8Array(hmac.arrayBuffer());
}

/**
 * Computes a PBKDF2 key block.
 *
 * @param prf - Pseudo-random function used to hash a block.
 * @param salt - Salt data.
 * @param rounds - Number of hash rounds in a key generation.
 * @param index - Index of a key block to compute.
 */
function pbkdf2(
  prf: (_: Uint8Array) => Uint8Array,
  salt: Uint8Array,
  rounds: number,
  index: number,
): Uint8Array {
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
