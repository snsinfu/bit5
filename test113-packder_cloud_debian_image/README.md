# Create Debian image and upload to Hetzner

Build an image:

```console
$ packer build build.pkr.hcl
$ vbox-img convert --srcfilename output-debian-live/*.vmdk --dstformat RAW --dstfilename debian.img
```

Create an Hetzner cloud instance. Reboot the instance in **rescue mode**. Let
`IP` be the IP address of the instance. Transport the image to the main disk
of the instance:

```console
$ gzip -c debian.img | ssh root@IP "gzip -cd | dd of=/dev/sda bs=32M"
```

Reboot the instance and the system will be up. Or, power off the instance and
take a snapshot of the instance. The snapshot would be usable as a reusable
base image for new instances.

## Notes

`cloud-utils` is required to resize partition and filesystem on boot. See:

- https://bugzilla.redhat.com/show_bug.cgi?id=1447177
- https://manpages.ubuntu.com/manpages/precise/man1/growpart.1.html
- https://cloudinit.readthedocs.io/en/latest/topics/modules.html

## TODO

ZFS on root. Does cloud-init work?
