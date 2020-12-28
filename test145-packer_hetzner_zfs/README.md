Create raw image.

```console
$ packer build build.pkr.hcl
$ vbox-img convert --srcfilename output-debian/*.vmdk --dstformat RAW --dstfilename output-debian/debian.img
```

Spin up a Hetzner cloud instance in rescue mode, and send the image to the
system.

```console
$ cd hetzner-snapshot
$ make
$ make ssh
(Check server is up)
$ make send_image
...
```

Go to Hetzner cloud console and take a snapshot of the server. Label it with
`custom_image=debian-10-zfs`. Test if the snapshot can be used as an image
and also if the cloud-init works:

```console
$ cd ../hetzner-use
$ make
$ make ssh
```

### FIXME

The zfs pool is not resized to fit to the disk. It seems growpart cloud-init
module is not working. Manual resizing works:

```console
$ sudo zfs list
NAME        USED  AVAIL     REFER  MOUNTPOINT
data        751M  1.02G       96K  /data
data/root   750M  1.02G      750M  /
$ sudo growpart /dev/sda 3
$ sudo zpool online data /dev/sda3
$ sudo reboot
...
$ sudo zfs list
NAME        USED  AVAIL     REFER  MOUNTPOINT
data        750M  35.7G       96K  /data
data/root   749M  35.7G      749M  /
```

So, it may be a configuration problem.


### Growpart fails

The cloudinit log reports this information:

```
2020-12-28 06:52:00,777 - cc_growpart.py[DEBUG]: '/' SKIPPED: stat of 'data/root' failed: [Errno 2] No such file or directory: 'data/root'
2020-12-28 06:52:00,777 - handlers.py[DEBUG]: finish: init-network/config-growpart: SUCCESS: config-growpart ran successfully
```

It says success but obviously something is wrong. Following `cc_resizefs` is
actually successful:

```
2020-12-28 06:52:00,779 - util.py[DEBUG]: Running command ['zpool', 'status', 'data'] with allowed return codes [0] (shell=False, capture=True)
2020-12-28 06:52:00,799 - util.py[DEBUG]: found zpool "data" on disk sda3
2020-12-28 06:52:00,799 - cc_resizefs.py[DEBUG]: resize_info: dev=sda3 mnt_point=/ path=data
...
2020-12-28 06:52:00,821 - cc_resizefs.py[DEBUG]: Resizing data (zfs) using zpool online -e data /dev/sda3
2020-12-28 06:52:00,821 - util.py[DEBUG]: Running command ('zpool', 'online', '-e', 'data', '/dev/sda3') with allowed return codes [0] (shell=False, capture=True)
2020-12-28 06:52:00,889 - util.py[DEBUG]: Resizing took 0.068 seconds
2020-12-28 06:52:00,889 - cc_resizefs.py[DEBUG]: Resized root filesystem (type=zfs, val=True)
```

So I guess the issue is in `cc_growpart`. The root filesystem points to zfs
dataset `zfs/pool` which in turn uses `sda3` as the backing partition, but
somehow `cc_growpart` could not recognize the device and failed to do its job:

```
hetzner@debian:~$ lsblk
NAME   MAJ:MIN RM  SIZE RO TYPE MOUNTPOINT
sda      8:0    0 38.2G  0 disk
├─sda1   8:1    0    1M  0 part
├─sda2   8:2    0  512M  0 part /boot
└─sda3   8:3    0    2G  0 part             <-- This should be ~38G
...
```

Related?

- https://bugs.launchpad.net/cloud-init/+bug/1799953
- https://github.com/canonical/cloud-utils/pull/9


### A fix

To correctly work, cloudinit needs to guess that (1) the zpool containing
(2) the dataset which is mounted to "/" (3) has only one device and (4) it's
resizable. Maybe that's too much to ask; it would be hard to make it work
robustly on various environment.

Following [the documentation][doc], I manually specified cloud-init to grow
`/dev/sda3` via cloud-config user data:

```yaml
growpart:
  mode: auto
  devices:
    - /dev/sda3
```

and it now works! Both the partition and zpool are resized to fit to the disk.

```
hetzner@debian:~$ lsblk
NAME   MAJ:MIN RM  SIZE RO TYPE MOUNTPOINT
sda      8:0    0 38.2G  0 disk
├─sda1   8:1    0    1M  0 part
├─sda2   8:2    0  512M  0 part /boot
└─sda3   8:3    0 37.7G  0 part
...
hetzner@debian:~$ sudo zfs list
NAME        USED  AVAIL     REFER  MOUNTPOINT
data        752M  35.7G       96K  /data
data/root   751M  35.7G      751M  /
```

Hard-coding `sda3` may be fragile especially when one attaches a volume to an
instance. IME volume is bound to `sdb` so it could be safe but I'm not sure.

[doc]: https://cloudinit.readthedocs.io/en/latest/topics/examples.html#grow-partitions
