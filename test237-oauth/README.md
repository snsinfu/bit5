# Using oauth-1.0a package with Deno

Using [oauth-1.0a](https://www.npmjs.com/package/oauth-1.0a) for constructing
[OAuth 1.0](https://tools.ietf.org/html/rfc5849) request header/body.

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

## Importing NPM module

The oauth-1.0a module provides `.d.ts` type declaration but is a CommonJS
module, so it is not straightforward to import the module from Deno. unpkg's
experimental `?module` feature almost works but type declaration is not loaded.
Here, I used [esm](https://esm.sh/) to import a typeful module. It works
smoothly, though I don't think it's a good idea to depend on arbitrarily
transmogrified module from a platform controlled by someone.

Edit: Skypack `?dts` feature also works.

```typescript
// Both works
import OAuth from "https://cdn.skypack.dev/oauth-1.0a@2.2.6?dts"
import OAuth from "https://esm.sh/oauth-1.0a@2.2.6";
```

But both are arbitrary third-party transformations anyway. I believe the import
URL should refer to the true source (GitHub in this case) and transformation
should be handled locally. [Integrity checking][lockfile] will break if CDN
changes how the module is transformed. Skypack does actually change polyfills
and config based on user-agent ("Browser-optimized imports").

Hope [node compatibility][nodecompat] will be improved in the future. For now,
I would [vendor (DENO_DIR)][vendoring] and [lock][lockfile] CDN-transformed
modules.

[lockfile]: https://deno.land/manual/linking_to_external_code/integrity_checking
[nodecompat]: https://github.com/denoland/deno/issues/2644
[vendoring]: https://deno.land/manual/linking_to_external_code
