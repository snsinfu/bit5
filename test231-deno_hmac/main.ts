import * as base64 from "https://deno.land/std@0.88.0/encoding/base64.ts";
import * as hex from "https://deno.land/std@0.88.0/encoding/hex.ts";
import { HmacSha1 } from "https://deno.land/std@0.88.0/hash/sha1.ts";

const secretKey = hex.decodeString("06540b8417d4568e0eae022dfd822333");
const hmac = new HmacSha1(secretKey);

hmac.update("The quick brown fox jumps over the lazy dog.");
const signature = base64.encode(new Uint8Array(hmac.digest()));

console.log(signature);
