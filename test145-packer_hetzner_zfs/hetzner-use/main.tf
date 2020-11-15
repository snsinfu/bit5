resource "hcloud_server" "main" {
  name        = "debian"
  location    = "fsn1"
  server_type = "cpx11"
  image       = data.hcloud_image.my_debian.id
  user_data   = file("${path.module}/cloudinit.yml")
}

data "hcloud_image" "my_debian" {
  with_selector = "custom_image=debian-10-zfs"
}
