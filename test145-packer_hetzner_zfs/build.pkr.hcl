source "virtualbox-iso" "debian" {
  guest_os_type = "Debian_64"
  iso_url = "https://cdimage.debian.org/debian-cd/10.6.0-live/amd64/iso-hybrid/debian-live-10.6.0-amd64-standard.iso"
  iso_checksum = "md5:3dc061d709e4a7c7bb119942fadcbed8"
  disk_size = 4096
  memory = 4096
  ssh_username = "user"
  ssh_password = "live"
  boot_command = [
    "<enter><wait10><wait5>",
    "sudo apt-get update",
    " && sudo apt-get install -y openssh-server",
    " && sudo systemctl start ssh",
    "<enter>",
  ]
  shutdown_command = "sudo shutdown -P now"
}

build {
  sources = ["sources.virtualbox-iso.debian"]

  provisioner "shell" {
    script = "setup.sh"
  }
}
