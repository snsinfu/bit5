#!/bin/sh -eux

if [ $(id -u) != 0 ]; then
    exec sudo "$0" "$@"
fi

DISK=/dev/sda
ROOT=/mnt
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

mount ${DISK}3 ${ROOT}
mkdir ${ROOT}/boot
mount ${DISK}2 ${ROOT}/boot

# DEBIAN SETUP ---------------------------------------------------------------

debootstrap ${CODENAME} ${ROOT}

mount -t proc  none ${ROOT}/proc
mount -t sysfs none ${ROOT}/sys
mount --bind   /dev ${ROOT}/dev

chroot ${ROOT} apt-get install -y --no-install-recommends grub-pc linux-image-amd64 locales
chroot ${ROOT} grub-install ${DISK}
chroot ${ROOT} update-grub

sed -i '/en_US\.UTF-8/ s/^# *//' ${ROOT}/etc/locale.gen

cat > ${ROOT}/etc/fstab << END
${DISK}2 /boot ext4  defaults 0 2
${DISK}3 /     ext4  defaults 0 1
proc     /proc proc  defaults 0 0
sysfs    /sys  sysfs defaults 0 0
END

cat > ${ROOT}/etc/network/interfaces.d/loopback << END
auto lo
iface lo inet loopback
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
