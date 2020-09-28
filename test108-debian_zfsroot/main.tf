resource "hcloud_server" "deb" {
  name        = "deb.example.com"
  location    = var.server_location
  server_type = var.server_type
  image       = var.server_image
  iso         = var.server_iso
  user_data   = data.template_file.install_script.rendered
}

data "template_file" "install_script" {
  template = ""
}
