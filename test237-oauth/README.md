# Using oauth-1.0a package with Deno

Using [oauth-1.0a](https://www.npmjs.com/package/oauth-1.0a) for constructing
[OAuth 1.0](https://tools.ietf.org/html/rfc5849) request header/body. The
package provides `.d.ts` type declaration but is a CommonJS module, so it is
not straightforward to import the modulet from a Deno source code. unpkg's
experimental `?module` feature almost works but type declaration is not loaded.
So, here I used [esm](https://esm.sh/) to import a typeful module. It works
smoothly, though I don't think it's a good idea to depend on arbitrarily
transmogrified module from a platform controlled by someone.

Example output:

```console
$ deno run main.ts
--- Data ---
{
  oauth_consumer_key: "25e65b1be60fff4e78aa32d69f4cc075",
  oauth_nonce: "Al9jxou7ZtWKRnaOoSYbOkknqBm1ul1r",
  oauth_signature_method: "HMAC-SHA1",
  oauth_timestamp: 1614964284,
  oauth_version: "1.0",
  scope: "read",
  oauth_signature: "dOWsODbWHjlghUNZ5irtQcHaLGo="
}

--- Header ---
Authorization: OAuth oauth_consumer_key="25e65b1be60fff4e78aa32d69f4cc075", oauth_nonce="Al9jxou7ZtWKRnaOoSYbOkknqBm1ul1r", oauth_signature="dOWsODbWHjlghUNZ5irtQcHaLGo%3D", oauth_signature_method="HMAC-SHA1", oauth_timestamp="1614964284", oauth_version="1.0"

--- Body ---
oauth_consumer_key=25e65b1be60fff4e78aa32d69f4cc075&oauth_nonce=Al9jxou7ZtWKRnaOoSYbOkknqBm1ul1r&oauth_signature_method=HMAC-SHA1&oauth_timestamp=1614964284&oauth_version=1.0&scope=read&oauth_signature=dOWsODbWHjlghUNZ5irtQcHaLGo%3D
```
