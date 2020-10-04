source "virtualbox-iso" "debian-live" {
  guest_os_type = "Debian_64"
  iso_url = "https://cdimage.debian.org/debian-cd/10.6.0-live/amd64/iso-hybrid/debian-live-10.6.0-amd64-standard.iso"
  iso_checksum = "md5:3dc061d709e4a7c7bb119942fadcbed8"
  ssh_username = "user"
  ssh_password = "live"
  boot_command = [
    "<wait10><enter><wait10>",
    "sudo apt-get update",
    " && ",
    "sudo apt-get install -y openssh-server",
    " && ",
    "sudo systemctl start ssh",
    "<enter>",
  ]
  shutdown_command = "sudo shutdown -P now"
}

build {
  sources = ["sources.virtualbox-iso.debian-live"]

  provisioner "shell" {
    inline = [
      "sudo dd if=/dev/zero of=/dev/sda bs=1M count=32",
      "echo '32MB written to the disk'",
      "sleep 5",
    ]
  }
}
