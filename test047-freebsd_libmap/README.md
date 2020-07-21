# FreeBSD: libmap to execute chrooted commands directly from host

Currently, FreeBSD does not support parallel installation of different versions
of PostgreSQL. However, to upgrade a PostgreSQL cluster to a newer release using
`pg_upgrade` command, there must be old and new PostgreSQL commands executable
in a single server.

The standard workarounnd is by (1) upgrading postgresql* package, (2) creating a
chroot containing old package, (3) and then use new `pg_upgrade` with correct
`-b` and `-B` options pointing to `/usr/local/bin` in the host and the chroot
environment.

- https://kreynolds.com/post/113982719240/freebsd-and-pgupgrade
- https://www.ogris.de/howtos/freebsd-postgresql-pg_upgrade.html

To do this correctly, one needs to map shared library paths used by the commands
in the chroot environment to the ones seen from the host environment. The second
link above does this right but finding out all libraries used by all commands
is cumbersome.

So, I consulted the man page of `libmap.conf` and found a better way to map
library search paths.

https://www.freebsd.org/cgi/man.cgi?query=libmap.conf


### Install old postgresql in a chroot environment

Install old postgresql binaries to a chroot `/pg`:

```console
$ sudo bsdinstall jail /pg
$ sudo pkg -r /pg install postgresql10-server
```

The installed commands can't be executed from the host environment. This is
because command executables in the chroot environment is linked against shared
libraries installed in the chroot environment. For example, `pg_dump` uses
`libpq` installed in the chroot, which can't be found in the library search
paths on the host environment:

```console
$ ldd /pg/usr/local/bin/pg_dump
/pg/usr/local/bin/pg_dump:
        libpq.so.5 => not found (0)
        libthr.so.3 => /lib/libthr.so.3 (0x8002a5000)
        libintl.so.8 => /usr/local/lib/libintl.so.8 (0x8002d2000)
        libz.so.6 => /lib/libz.so.6 (0x8002df000)
        libc.so.7 => /lib/libc.so.7 (0x8002f9000)
```


### Map library search paths

This issue can be resolved by mapping library search paths for the binaries in
the chroot environment to the ones prefixed by `/pg` on the host environment.
Let's add a libmap config on the host:

```console
$ sudo mkdir /usr/local/etc/libmap.d
$ sudo vi /usr/local/etc/libmap.d/pg.conf
```

The content should look like this:

```
[/pg/]
/lib           /pg/lib
/usr/lib       /pg/usr/lib
/usr/local/lib /pg/usr/local/lib
```

This configuration instructs the loader that, when loading binaries under `/pg/`
tree, transform standard library search paths to ones prefixed by `/pg`. Now,
the `pg_dump` command installed in the chroot environment can be executed
directly on the host environment thanks to the mapping:

```console
$ /pg/usr/local/bin/pg_dump --version
pg_dump (PostgreSQL) 10.13

$ ldd /pg/usr/local/bin/pg_dump
/pg/usr/local/bin/pg_dump:
        libpq.so.5 => /pg/usr/local/lib/libpq.so.5 (0x8002a5000)
        libthr.so.3 => /pg/lib/libthr.so.3 (0x8002f0000)
        libintl.so.8 => /pg/usr/local/lib/libintl.so.8 (0x80031d000)
        libz.so.6 => /pg/lib/libz.so.6 (0x80032b000)
        libc.so.7 => /pg/lib/libc.so.7 (0x800345000)
        libssl.so.111 => /pg/usr/lib/libssl.so.111 (0x80073e000)
        libcrypto.so.111 => /pg/lib/libcrypto.so.111 (0x8007d6000)
```

Note how all the library paths resolve to the ones under `/pg`.


### Clean up

`bsdinstall` sets `schg` flag to some parts of the chroot tree. Make sure
clearing the flag before removing the chroot tree.

```console
$ sudo chflags -R noschg /pg
$ sudo rm -rf /pg
```
