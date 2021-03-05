# OAuth 1.0a signature base string

https://tools.ietf.org/html/rfc5849

At first glance, I thought that the standard just uses the standard percent
encoding and form data encoding. So, it felt trivial for me to implement an
OAuth 1.0a client by using `URLSearchParams` and `encodeURIComponent`. However,
reading RFC5849 carefully, I noticed that the standard uses [a special dialect
][percent] of percent encoding and requires [a special normalization][norm] to
the encoded parameters. It's not trivial at all! Now I understand why packages
like [oauth-1.0a][npm-oauth1a] are needed and why these packages implement
their own percent encoding logics.

[percent]: https://tools.ietf.org/html/rfc5849#section-3.6
[norm]: https://tools.ietf.org/html/rfc5849#section-3.4.1.3.2
[npm-oauth1a]: https://www.npmjs.com/package/oauth-1.0a
