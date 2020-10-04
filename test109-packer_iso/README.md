Using Packer to manipulate Debian Live CD on VirtlaBox. ISO cache is stored in
`packer_cache` directory and virtual machine is stored in `output-debian-live`
directory.

```console
$ packer build build.pkr.hcl
```

I want to convert VMDK disk image to RAW format without creating file (instead
I'd like to stream raw image to another computer). But, this did not work:

```console
$ vbox-img convert --srcfilename output-debian-live/packer-debian-live-1601775668-disk001.vmdk --stdout --dstformat RAW | gzip | wc -c
Converting image "packer-debian-live-1601775668-disk001.vmdk" with size 41943040000 bytes (40000MB)...
vbox-img: error: Error while copying the image: Not supported. (VERR_NOT_SUPPORTED)
   16329
```

`--dstformat VMDK --variant Stream` works. However, RAW format does not accept
Stream variant.
