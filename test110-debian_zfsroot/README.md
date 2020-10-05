https://openzfs.github.io/openzfs-docs/Getting%20Started/Debian/Debian%20Buster%20Root%20on%20ZFS.html

```console
$ packer build build.pkr.hcl
...
$ vbox-img convert --srcfilename output-debian-live/*.vmdk --dstformat RAW --dstfilename disk.img
```

As of now, debootstrap runs successfully on zfsroot. Other initial
configurations, including cloudinit bootstrapping, should additionally done.
Then, I'd upload the disk image to a cloud VPS, snapshot the instance and use
it as a ZFS-root Debian template.
