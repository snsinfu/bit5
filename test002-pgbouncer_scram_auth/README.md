# PgBouncer SCRAM server auth

Recent release of PgBouncer added SCRAM support:

- https://www.pgbouncer.org/2020/06/pgbouncer-1-14-0
- https://www.pgbouncer.org/changelog.html#pgbouncer-114x

Now I do not have to store plaintext passwords in userlist.txt. Great! It's
already in FreeBSD's ports collection and 13-CURRENT package builds, so let's
try it out in a virtual FreeBSD machine.

To test:

```console
$ vagrant up
```

Note: FreeBSD's official vagrant box updates the system on first boot, so
`vagrant up` takes several minutes showing "Connection reset. Retrying..."
messages. Run `vagrant provision` manually if it timeouts.

SCRAM auth works. Some notes:

- Make sure to pass `-A scram-sha-256` to initdb
- Ansible updates DB user password every time probably due to random salt
- PgBouncer authfile is retrieved from `pg_shadow` table; is this really
  the right way?
