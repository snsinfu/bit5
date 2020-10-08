#!/bin/sh -eux

if [ $(id -u) != 0 ]; then
    exec sudo "$0" "$@"
fi

DISK=/dev/sda
CODENAME=buster

export DEBIAN_FRONTEND=noninteractive

apt-get update
apt-get install -y --no-install-recommends debootstrap gdisk

# DISK -----------------------------------------------------------------------

sgdisk -a1 -n1:24K:+1000K -t1:EF02 ${DISK}  # BIOS
sgdisk     -n2:0:+1G      -t2:8300 ${DISK}  # Boot
sgdisk     -n3:0:0        -t3:8304 ${DISK}  # Root

mkfs.ext4 ${DISK}2
mkfs.ext4 ${DISK}3

mount ${DISK}3 /mnt
mkdir /mnt/boot
mount ${DISK}2 /mnt/boot

# System ---------------------------------------------------------------------

debootstrap ${CODENAME} /mnt

mount -t proc  none /mnt/proc
mount -t sysfs none /mnt/sys
mount --bind   /dev /mnt/dev

chroot /mnt

apt-get install -y --no-install-recommends \
    cloud-init \
    grub-pc \
    linux-image-amd64 \
    openssh-server

cat > /etc/fstab << END
${DISK}2 /boot ext4  defaults 0 2
${DISK}3 /     ext4  defaults 0 1
proc     /proc proc  defaults 0 0
sysfs    /sys  sysfs defaults 0 0
END

cat > /etc/ssh/sshd_config << END
PermitRootLogin no
PasswordAuthentication no
ChallengeResponseAuthentication no
UsePAM yes
Subsystem sftp /usr/lib/openssh/sftp-server
END

cat > /etc/network/interfaces.d/setup << END
auto lo
iface lo inet loopback

auto enp0s3
allow-hotplug enp0s3
iface enp0s3 inet dhcp
END

# This fails. "grub-install: error: failed to get canonical path of `overlay'."
grub-install ${DISK}
update-grub

passwd -d root

sleep 3600
