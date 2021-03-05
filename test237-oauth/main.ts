import * as base64 from "https://deno.land/std@0.89.0/encoding/base64.ts";
import { HmacSha1 } from "https://deno.land/std@0.89.0/hash/sha1.ts";
import OAuth from "https://esm.sh/oauth-1.0a@2.2.6";

const oauth = new OAuth({
  consumer: {
    key: "25e65b1be60fff4e78aa32d69f4cc075",
    secret: "96fa01b6e880942654e49e4e23d77415",
  },
  signature_method: "HMAC-SHA1",
  hash_function(baseString: string, key: string): string {
    const hmac = new HmacSha1(key);
    hmac.update(baseString);
    return base64.encode(hmac.arrayBuffer());
  },
});

const request: OAuth.RequestOptions = {
  method: "POST",
  url: "https://api.example.com/authorize",
  data: {
    scope: "read",
  },
};

const data = oauth.authorize(request);
const auth = oauth.toHeader(data).Authorization;
const body = new URLSearchParams(data as any);

console.log("--- Data ---");
console.log(data);
console.log();
console.log("--- Header ---");
console.log("Authorization:", auth);
console.log();
console.log("--- Body ---");
console.log(body.toString());
