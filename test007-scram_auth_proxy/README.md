# SCRAM authentication via proxy

SCRAM protocol allows server (including proxy) to derive `ClientKey` from proof
sent by client on authentication process. That `ClientKey` is sufficient to
compute a new client proof for arbitrary message. So, proxy works.

Run demo:

```console
$ python3 main.py
```

## The authentication process

First, Client and Server share `AuthMessage`. Client computes a proof:

```
Client (knows ClientKey):
  StoredKey = H(ClientKey)
  ClientSignature = HMAC(StoredKey, AuthMessage)
  ClientProof = ClientKey XOR ClientSignature
```

Client sends `ClientProof` to Server, and Server derives `ClientKey`.

```
Server (knows StoredKey):
  ClientSignature = HMAC(StoredKey, AuthMessage)
  ClientKey = ClientProof XOR ClientSignature
```

Now, Server verifies Client by checking if `H(ClientKey) == StoredKey`.

Since Server knows `ClientKey` and `StoredKey`, it can generate new `ClientProof`
for another server as if it is a legitimate client. So, a proxy server can
authenticate user against an origin server without knowing plaintext password.
This is why PgBouncer's SCRAM auth works even if only hashed secrets are stored
in `auth_file`.

Note that, to derive `ClientKey`, the proxy needs to use the same `StoredKey`
as origin server. Since `StoredKey` depends on salt and hash rounds, the proxy
cannot change these parameters. Hence, PgBouncer's `auth_file` entries need to
exactly match the entries in PostgreSQL's `pg_shadow` table.
