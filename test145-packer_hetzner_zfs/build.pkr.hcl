source "virtualbox-iso" "debian" {
  guest_os_type = "Debian_64"
  iso_url = "https://cdimage.debian.org/debian-cd/10.7.0-live/amd64/iso-hybrid/debian-live-10.7.0-amd64-standard.iso"
  iso_checksum = "md5:0e5c1aad9a32b7b009f2a7f8f1590320"
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
