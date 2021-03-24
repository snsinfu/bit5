import * as http from "https://deno.land/std@0.91.0/http/mod.ts";
import {
  assert,
  assertEquals,
} from "https://deno.land/std@0.91.0/testing/asserts.ts";

Deno.test("request is recorded", async () => {
  // Temporary server.
  const port = 29513;
  const server = http.serve({ hostname: "localhost", port: port });

  // Record requests. We asynchronously launch a recording loop and forget it.
  // The loop ends when the server is closed.
  interface RequestData {
    method: string;
    path: string;
    headers: Headers;
    body: ArrayBuffer;
  }
  const requests: RequestData[] = [];

  (async () => {
    for await (const request of server) {
      requests.push({
        method: request.method,
        path: request.url,
        headers: request.headers,
        body: await Deno.readAll(request.body),
      });
      request.respond({ status: 200 });
    }
  })();

  // Test a function that makes an HTTP request to the temporary server. Here
  // we use fetch() as an example.
  const method = "POST";
  const path = "/review/100";
  const bodyData = { score: "3", comment: "ok" };
  const mimeType = "application/x-www-form-urlencoded";

  const response = await fetch(`http://localhost:${port}${path}`, {
    method: method,
    headers: { "Content-Type": mimeType },
    body: new URLSearchParams(bodyData),
  });
  await response.arrayBuffer();

  // Inspect the recorded request and test if the request data is as expected.
  assertEquals(requests.length, 1);

  const actual = requests[0];

  assertEquals(actual.method, method);
  assertEquals(actual.path, path);
  assertEquals(actual.headers.get("Content-Type"), mimeType);

  const utf8 = new TextDecoder("utf-8");
  const params = new URLSearchParams(utf8.decode(actual.body));

  assertEquals(Array.from(params.entries()), Object.entries(bodyData));

  // Must close the temporary server.
  server.close();
});
