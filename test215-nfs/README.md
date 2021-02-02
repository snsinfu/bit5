# NFSv4 on Debian server and client

Provision server and client:

```console
$ vagrant up
```

NFSv4 idmapping is disabled by default:

```console
$ vagrant ssh server
$ cat /sys/module/nfsd/parameters/nfs4_disable_idmapping
Y
```

It's documented in the [admin guide][1] (search for `nfs4_disable_idmapping`).
Because of this, a file can be created on the NFS shared volume by a user that
does not exist on the server. See:

```console
$ vagrant ssh server
$ ls -l /srv/nfs/foo
total 12
-rw-r--r-- 1 9999 9999 8379 Feb  2 14:38 facts.txt
```

The playbook configures NFS server with no access control. Use `netstat` to
find out the client IP (in this case `172.29.1.11`). This will be useful to
set up NFS server for systems like k3s.

```console
$ sudo apt install net-tools
$ netstat -t
Active Internet connections (w/o servers)
Proto Recv-Q Send-Q Local Address           Foreign Address         State
tcp        0      0 10.0.2.15:ssh           10.0.2.2:53557          ESTABLISHED
tcp        0      0 172.29.1.10:nfs         172.29.1.11:678         ESTABLISHED
```

So, you may reconfigure NFS server to only accept that IP (or IP range):

```console
$ vagrant ssh server
$ cat > /etc/exports
/srv/nfs 172.29.1.0/24(rw,fsid=0,no_subtree_check)
^D
$ sudo systemctl restart nfs-server
```

[1]: https://www.kernel.org/doc/html/v4.19/admin-guide/kernel-parameters.html
