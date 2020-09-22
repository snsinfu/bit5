# TOTP

- TOTP: https://tools.ietf.org/html/rfc6238
- HOTP: https://tools.ietf.org/html/rfc4226
- HMAC: https://tools.ietf.org/html/rfc2104

```
TOTP = HOTP(K, T)
HOTP(K, C) = Truncate(HMAC-SHA-1(K, C))
HMAC-H(K, C) = H(K XOR opad | H(K XOR ipad | C))`

where
  T = (Current Unix time - T0) / X
  ipad = 0x36 ...
  opad = 0x5C ...
```
