resource "hcloud_server" "main" {
  name        = "debian"
  location    = "fsn1"
  server_type = "cpx11"
  image       = "debian-10"
  rescue      = "linux64"
  ssh_keys    = data.hcloud_ssh_keys.defaults.ssh_keys.*.name
}

data "hcloud_ssh_keys" "defaults" {
  with_selector = "default=true"
}
