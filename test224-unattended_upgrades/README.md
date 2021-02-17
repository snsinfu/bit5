# Debian unattended upgrades

```console
$ vagrant up
```

Log in to the machine and run unattended-upgrades:

```console
$ vagrant ssh
$ sudo unattended-upgrades -d
...
```

This would upgrade the kernel. So reboot is needed, but unattended-upgrades
does not reboot the system by default. You can configure it to automatically
reboot the system:

```
Unattended-Upgrade::Automatic-Reboot "true";
```

But I did not configured it in this example. If you do not, you may want to
check the existence of `/var/runn/reboot-required`.
