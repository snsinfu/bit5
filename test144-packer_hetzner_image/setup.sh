#!/bin/sh -eux

DISK=/dev/sda
ROOT=/mnt
ROOTSIZE=10G
CODENAME=buster

if [ $(id -u) != 0 ]; then
    exec sudo "$0" "$@"
fi

export DEBIAN_FRONTEND=noninteractive

apt-get update
apt-get install -y --no-install-recommends debootstrap gdisk

# DISK -----------------------------------------------------------------------

sgdisk -a1 -n1:24K:+1M        -t1:EF02 ${DISK}  # BIOS
sgdisk     -n2:0:+${ROOTSIZE} -t2:8300 ${DISK}  # Root partition
sgdisk     -n3:0:0            -t2:8300 ${DISK}  # Free space

mkfs.ext4 ${DISK}2
mount ${DISK}2 ${ROOT}

# DEBIAN SETUP ---------------------------------------------------------------

debootstrap ${CODENAME} ${ROOT}

mount -t proc  none ${ROOT}/proc
mount -t sysfs none ${ROOT}/sys
mount --bind   /dev ${ROOT}/dev

chroot ${ROOT} apt-get install -y --no-install-recommends locales
sed -i '/en_US\.UTF-8/ s/^# *//' ${ROOT}/etc/locale.gen
chroot ${ROOT} locale-gen

chroot ${ROOT} apt-get install -y --no-install-recommends grub-pc linux-image-amd64
chroot ${ROOT} grub-install ${DISK}
chroot ${ROOT} update-grub

cat > ${ROOT}/etc/fstab << END
${DISK}2 /     ext4  defaults 0 1
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
