Create instance:

```console
$ export HCLOUD_TOKEN=...
$ make
```

Open Hetzner Cloud console and reboot the instance in linux rescue mode with
your SSH key enabled. Log in to the rescue system.

```console
$ make ssh

# parted -l
Number  Start   End     Size    File system  Name  Flags
14      1049kB  2097kB  1049kB                     bios_grub
15      2097kB  130MB   128MB   fat32              boot, esp
 1      130MB   41.0GB  40.8GB  ext4

# parted -- /dev/sda resizepart 1 -30G
# parted -- /dev/sda mkpart primary zfs -30G 100%
# partprobe
# e2fsck -f /dev/sda1
Either the superblock or the partition table is likely to be corrupt!
Abort<y>?
```

Looks like, shrinking partition trashes the filesystem... It'd be better off
installing Debian from scratch with custom partitioning.
