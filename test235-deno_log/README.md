# Deno Oak / Logging

Run server:

```console
$ deno run --allow-net=localhost main.ts
```

Curl some endpoints:

```console
$ curl -f http://localhost:8080/
{"message":"OK"}
$ curl -f http://localhost:8080/a/b/c
curl: (22) The requested URL returned error: 404 Not Found
$ curl -f http://localhost:8080/crash
curl: (22) The requested URL returned error: 500 Internal Server Error
```

The server log would look like this:

```
2021-03-04T12:11:37.687Z [INFO] Listening on localhost:8080
2021-03-04T12:11:56.793Z [INFO] 200 => GET / (from ::1)
2021-03-04T12:11:59.441Z [WARNING] 404 => GET /a/b/c (from ::1)
2021-03-04T12:12:03.177Z [ERROR] XXX => GET /crash (from ::1)
2021-03-04T12:12:03.182Z [ERROR] Error: crash
    at router.ts:12:9
    at dispatch (middleware.ts:41:13)
    at router.ts:874:20
    at dispatch (middleware.ts:41:13)
    at composedMiddleware (middleware.ts:44:12)
    at dispatch (router.ts:880:28)
    at dispatch (middleware.ts:41:13)
    at main.ts:37:11
    at dispatch (middleware.ts:41:13)
    at composedMiddleware (middleware.ts:44:12)
```
