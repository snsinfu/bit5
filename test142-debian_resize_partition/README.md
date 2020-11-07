Create innstance:

```console
$ export HCLOUD_TOKEN=...
$ make
$ make ssh
```

Connect to the machine and on-line shrink the main partition (dangerous!).

```console
$ make ssh
...

# apt update
# apt install parted
# parted

(parted) print all
...
Number  Start   End     Size    File system  Name  Flags
14      1049kB  2097kB  1049kB                     bios_grub
15      2097kB  130MB   128MB   fat32              boot, esp
 1      130MB   41.0GB  40.8GB  ext4

(parted) resizepart 1
...
End?  [41.0GB]? 9GB

(parted) mkpart primary zfs 9004MB 100%

(parted) quit

# reboot
```

And the machine won't boot! Okay, do not live-shrink partition.

Cleanup:

```console
$ make destroy
$ make clean
```
