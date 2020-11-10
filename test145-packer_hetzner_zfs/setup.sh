#!/bin/sh -eux

DISK=/dev/sda
ROOT=/mnt
POOL=data
BOOTSIZE=512M
POOLSIZE=2G
CODENAME=buster
ARCH=$(uname -r)

if [ $(id -u) != 0 ]; then
    exec sudo "$0" "$@"
fi

export DEBIAN_FRONTEND=noninteractive

cat > /etc/apt/sources.list <<END
deb http://deb.debian.org/debian buster main contrib
deb http://deb.debian.org/debian buster-backports main contrib
END

apt-get update
apt-get install -y --no-install-recommends debootstrap gdisk

# ZoL for livecd environment.
apt-get install -y --no-install-recommends dkms dpkg-dev linux-headers-${ARCH}
apt-get install -y --no-install-recommends -t buster-backports zfs-dkms
modprobe zfs
apt-get install -y --no-install-recommends -t buster-backports zfsutils-linux

# DISK -----------------------------------------------------------------------

sgdisk -a1 -n1:24K:+1M        -t1:EF02 ${DISK}  # BIOS
sgdisk     -n2:0:+${BOOTSIZE} -t2:8300 ${DISK}  # Boot partition
sgdisk     -n3:0:+${POOLSIZE} -t2:8300 ${DISK}  # zpool

zpool create \
  -o ashift=12 \
  -O acltype=posixacl \
  -O canmount=off \
  -O compression=lz4 \
  -O relatime=on \
  -R ${ROOT} \
  ${POOL} \
  ${DISK}3

zfs create -o mountpoint=/ ${POOL}/root

mkdir ${ROOT}/boot

mkfs.ext4 ${DISK}2
mount ${DISK}2 ${ROOT}/boot

# DEBIAN SETUP ---------------------------------------------------------------

debootstrap ${CODENAME} ${ROOT}

mount -t proc  none ${ROOT}/proc
mount -t sysfs none ${ROOT}/sys
mount --bind   /dev ${ROOT}/dev

chroot ${ROOT} ln -s /proc/self/mounts /etc/mtab

cat > ${ROOT}/etc/apt/sources.list <<END
deb     http://deb.debian.org/debian/          ${CODENAME}           main contrib
deb-src http://deb.debian.org/debian/          ${CODENAME}           main contrib
deb     http://deb.debian.org/debian/          ${CODENAME}-backports main contrib
deb-src http://deb.debian.org/debian/          ${CODENAME}-backports main contrib
deb     http://deb.debian.org/debian-security/ ${CODENAME}/updates   main
deb-src http://deb.debian.org/debian-security/ ${CODENAME}/updates   main
deb     http://deb.debian.org/debian/          ${CODENAME}-updates   main
deb-src http://deb.debian.org/debian/          ${CODENAME}-updates   main
END
chroot ${ROOT} apt-get update

# Need to set up en_US to suppress locale-related warnings in installation.
chroot ${ROOT} apt-get install -y --no-install-recommends locales
sed -i '/en_US\.UTF-8/ s/^# *//' ${ROOT}/etc/locale.gen
chroot ${ROOT} locale-gen

# ZoL for installed environment
chroot ${ROOT} apt-get install -y --no-install-recommends dpkg-dev linux-headers-${ARCH} linux-image-${ARCH}
chroot ${ROOT} apt-get install -y --no-install-recommends -t buster-backports spl spl-dkms zfs-initramfs zfsutils-linux
chroot ${ROOT} apt-get install -y --no-install-recommends -t buster-backports zfsutils-linux
echo REMAKE_INITRD=yes > /etc/dkms/zfs.conf

chroot ${ROOT} apt-get install -y --no-install-recommends grub-pc
chroot ${ROOT} grub-install ${DISK}
chroot ${ROOT} update-grub

cat > ${ROOT}/etc/fstab << END
${DISK}2 /boot ext4  defaults 0 1
proc     /proc proc  defaults 0 0
sysfs    /sys  sysfs defaults 0 0
END

# MINIMAL SYSTEM FOR CLOUDINIT -----------------------------------------------

chroot ${ROOT} apt-get install -y --no-install-recommends cloud-init openssh-server
chroot ${ROOT} passwd -d root

cat > ${ROOT}/etc/ssh/sshd_config << END
PermitRootLogin no
PasswordAuthentication no
ChallengeResponseAuthentication no
UsePAM yes
Subsystem sftp /usr/lib/openssh/sftp-server
END

chroot ${ROOT} systemctl enable cloud-init-local
chroot ${ROOT} systemctl enable cloud-init
chroot ${ROOT} systemctl enable cloud-config
chroot ${ROOT} systemctl enable cloud-final

# CLEANUP --------------------------------------------------------------------

umount ${ROOT}/boot
umount ${ROOT}/proc
umount ${ROOT}/sys
umount ${ROOT}/dev
umount ${ROOT}
zpool export -a
