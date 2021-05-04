const host = "localhost";
const port = 3000;
const server = Deno.listen({ hostname: host, port: port });

console.log(`Listening on http://${host}:${port}/`);

for await (const conn of server) {
  (async () => {
    console.log("* Connection", conn.rid, "from", conn.remoteAddr);

    const httpConn = Deno.serveHttp(conn);

    for await (const requestEvent of httpConn) {
      const { request } = requestEvent;

      function summarize(request: Request): string {
        const { method, url, headers } = request;
        let body = `${method} ${url}\n`;
        for (const [key, value] of headers.entries()) {
          body += `${key}: ${value}\n`;
        }
        return body;
      }

      const response = new Response(
        summarize(request),
        {
          status: 200,
          headers: { "cntent-type": "text/plain" },
        },
      );

      requestEvent.respondWith(response);
    }

    console.log("* Connection", conn.rid, "done");
  })();
}
