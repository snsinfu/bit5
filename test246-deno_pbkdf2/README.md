10000-round PBKDF with HMAC-SHA256 as the PRF. Deno implementation, Python
hashlib as a reference.

```console
$ python3 main.py
f1951182eb7afc11e63f2784eb430b15c171ae826f0748d1bc4c974f5ac64ad2
$ deno run main.ts
f1951182eb7afc11e63f2784eb430b15c171ae826f0748d1bc4c974f5ac64ad2
```
