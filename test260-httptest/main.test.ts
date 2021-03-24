import * as http from "https://deno.land/std@0.91.0/http/mod.ts";
import {
  assert,
  assertEquals,
} from "https://deno.land/std@0.91.0/testing/asserts.ts";

Deno.test("request is recorded", async () => {
  const PORT = 29513;
  const MIME_FORM_ENCODED = "application/x-www-form-urlencoded";

  const server = http.serve({ hostname: "localhost", port: PORT });
  const response = fetch(`http://localhost:${PORT}/review/100`, {
    method: "POST",
    headers: { "Content-Type": MIME_FORM_ENCODED },
    body: new URLSearchParams({ score: "3", comment: "ok" }),
  });

  let actual: http.ServerRequest | undefined;
  for await (const request of server) {
    actual = request;
    request.respond({ status: 200 });
    break;
  }
  await (await response).arrayBuffer();
  server.close();

  assert(actual);
  assertEquals(actual.method, "POST");
  assertEquals(actual.url, "/review/100");
  assertEquals(actual.headers.get("Content-Type"), MIME_FORM_ENCODED);
  assert(actual.body);

  const body = await Deno.readAll(actual.body);
  const utf8 = new TextDecoder("utf-8");
  const params = new URLSearchParams(utf8.decode(body));

  assertEquals(
    Array.from(params.entries()),
    [["score", "3"], ["comment", "ok"]],
  );
});
