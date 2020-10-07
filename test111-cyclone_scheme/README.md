Did not work on macOS/Homebrew. Compile fails with error:

```
Error: Unable to open file:
"/usr/local/share/cyclone/scheme/cyclone/common.sld"
```

This could be worked around by manually creating a symlink:

```console
$ ln -s /usr/local/share/cyclone/scheme/{common.sld,cyclone/}
```

However, link fails.

```
ld: warning: ignoring file /usr/local/lib/libcyclonebn.a, building for macOS-x86_64 but attempting to link with file built for unknown-unsupported file format ( 0x21 0x3C 0x61 0x72 0x63 0x68 0x3E 0x0A 0x2F 0x20 0x20 0x20 0x20 0x20 0x20 0x20 )
Undefined symbols for architecture x86_64:
  "_mp_abs", referenced from:
        ___lambda_1343 in base.o
...
```

Bootstrap failure?
