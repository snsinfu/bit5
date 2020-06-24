Set up a FreeBSD system with monit installed:

```console
vagrant up
```

Simulate excess load:

```console
$ vagrant ssh
$ sudo pkg install stress-ng
$ stress-ng --cpu 2 --timeout 60s
```

Monitor the log and see how system alert gets logged:

```console
$ vagrant ssh
$ tail -f /var/log/monit.log
[UTC Jun 24 13:34:06] info     : 'vagrant' Monit reloaded
[UTC Jun 24 13:35:06] warning  : 'vagrant' cpu usage of 100.0% matches resource limit [cpu usage > 95.0%]
```

Looks like E-mail alert is available in the free version. M/Monit (self-hosted
web monitoring software) requires a paid license.
