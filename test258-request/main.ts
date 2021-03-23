import * as base64 from "https://deno.land/std@0.90.0/encoding/base64.ts";
import { Sha1 } from "https://deno.land/std@0.90.0/hash/sha1.ts";

const MIME_FORM_ENCODED = "application/x-www-form-urlencoded";

async function sign(request: Request): Promise<Request> {
  const result = request.clone();

  const body = request.headers.get("Content-Type") === MIME_FORM_ENCODED
    ? new URLSearchParams(await request.text())
    : undefined;

  if (body) {
    type KV = [string, string];

    const normBody = Array.from(body)
      .map(([key, value]: KV): KV => {
        return [encodeURIComponent(key), encodeURIComponent(value)];
      })
      .sort();

    const baseString = normBody
      .map(([key, value]: KV): string => key + "=" + value)
      .join("=");

    const hash = new Sha1();
    hash.update(baseString);
    const bodyHash = base64.encode(hash.arrayBuffer());

    result.headers.set("X-Body-Hash", bodyHash);
  }

  return result;
}

const request = await sign(
  new Request(
    "https://api.example.com/profile",
    {
      method: "PATCH",
      body: new URLSearchParams({ status: "busy", label: "green" }),
      headers: { "Content-Type": "application/x-www-form-urlencoded" },
    },
  ),
);
console.log(request.headers);
console.log(await request.text());
