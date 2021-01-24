# Binding to Wireguard IP address

Start server and client machines:

```console
$ vagrant up
```

This installs nginx to the server. Nginx binds to the IP address associated
with the wireguard interface `wg0`. It... works.

Let us ssh into the server:

```console
$ vagrant ssh server
```

Stopping `wg-quick` does not disrupt the running service immediately:

```console
$ sudo systemctl stop wg-quick@wg0
$ sudo journalctl -u nginx
Jan 24 13:48:04 server systemd[1]: Started A high performance web server and a reverse pro
```

Of course, the client machine cannot access to the server though. Now, try
restarting nginx. It fails:

```console
$ sudo systemctl restart nginx
Job for nginx.service failed because the control process exited with error code.
See "systemctl status nginx.service" and "journalctl -xe" for details.
$ sudo jounalctl -u nginx
...
Jan 24 14:04:45 server nginx[16490]: nginx: [emerg] bind() to 10.120.5.10:80 failed (99: C
Jan 24 14:04:45 server nginx[16490]: nginx: configuration file /etc/nginx/nginx.conf test
...
)
```

Restarting `wg-quick` fixes the situation:

```console
$ sudo systemctl start wg-quick@wg0
$ sudo systemctl start nginx
```

The client now can access to the server.


## Conclusion

It's ok to listen on wireguard IP address, and also it's ok to quickly restart
`wg-quick` while the server is running (at least for an idle nginx).

However, it's not robust. If the server stops and restarts before `wg-quick`
does, the server will fail. So, maybe the server should listen on a more stable
interface. Networking through wireguard is still possible if we setup routing
information correctly.
