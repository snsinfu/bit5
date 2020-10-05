#!/bin/sh -eux

if [ $(id -u) != 0 ]; then
    exec sudo "$0" "$@"
fi

export DEBIAN_FRONTEND=noninteractive

dd of=/etc/apt/sources.list <<END
deb http://deb.debian.org/debian buster main contrib
deb http://deb.debian.org/debian buster-backports main contrib
END

apt-get update
apt-get install -y --no-install-recommends debootstrap gdisk dkms dpkg-dev linux-headers-$(uname -r)
apt-get install -y --no-install-recommends -t buster-backports zfs-dkms
modprobe zfs
apt-get install -y --no-install-recommends -t buster-backports zfsutils-linux

DISK=/dev/sda

sgdisk -a1 -n1:24K:+1000K -t1:EF02 ${DISK}
sgdisk     -n2:0:0        -t2:BF00 ${DISK}

zpool create -O canmount=off -O compression=lz4 -R /mnt tank ${DISK}2
zfs create -o mountpoint=/     tank/root
zfs create -o mountpoint=/boot tank/boot

debootstrap buster /mnt
