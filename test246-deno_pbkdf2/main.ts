import * as hex from "https://deno.land/std@0.88.0/encoding/hex.ts";
import { HmacSha256 } from "https://deno.land/std@0.88.0/hash/sha256.ts";

type PRF = (_: Uint8Array) => Uint8Array;

function pbkdf2(prf: PRF, salt: Uint8Array, rounds: number, index: number): Uint8Array {
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

function makeHmacSha256(key: Uint8Array): PRF {
  return (message: Uint8Array): Uint8Array => {
    const hmac = new HmacSha256(key);
    hmac.update(message);
    return new Uint8Array(hmac.arrayBuffer());
  };
}

const utf8 = new TextEncoder();
const password = "password";
const salt = "9a47f7cd8885a288514ec835fa8a8c84";
const rounds = 10000;
const prf = makeHmacSha256(utf8.encode(password));
const key = pbkdf2(prf, hex.decodeString(salt), rounds, 1);
console.log(hex.encodeToString(key));
