import * as hex from "https://deno.land/std@0.89.0/encoding/hex.ts";

function makeSignatureBaseString(method: string, url: URL, params: any): string {

    // No, this implementation is not conformant. The standard requires special
    // percent encoding and normalization. We cannot use these standard tools.

    let q = new URLSearchParams();
    for (const key in params) {
        q.set(key, params[key]);
    }

    let s = "";
    s += method;
    s += "&";
    s += encodeURIComponent(url.toString());
    s += "&";
    s += encodeURIComponent(q.toString());

    return s;
}

function timestamp(): number {
    return (Date.now() / 1000) >> 0;
}

function nonce(): string {
    const data = crypto.getRandomValues(new Uint8Array(16));
    return hex.encodeToString(data);
}

const data = makeSignatureBaseString("POST", new URL("https://api.example.com/auth"), {
    oauth_callback: "https://app.example.com/callback",
    oauth_consumer_key: "212f8dfd8f95176f53b62ea3560590a5",
    oauth_signature_method: "HMAC-SHA1",
    oauth_timestamp: timestamp(),
    oauth_nonce: nonce(),
});
console.log(data);
