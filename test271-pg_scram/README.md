Run PostgreSQL with SCRAM authentication enabled:

```console
$ docker run -it --rm -e POSTGRES_USER=test -e POSTGRES_PASSWORD=test -e POSTGRES_DB=test -e POSTGRES_HOST_AUTH_METHOD=scram-sha-256 -e POSTGRES_INITDB_ARGS=--auth-host=scram-sha-256 -p 5432:5432 postgres
```

Then, run script.

```console
$ deno run --unstable --allow-net=localhost main.ts
```
