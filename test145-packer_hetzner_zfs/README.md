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
