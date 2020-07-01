terraform {
  backend "s3" {}
}

resource "hcloud_server" "test" {
  name        = var.server_hostname
  location    = var.server_location
  server_type = var.server_type
  image       = var.server_image
  user_data   = data.template_file.cloudinit.rendered
}

data "template_file" "cloudinit" {
  template = file("${path.module}/assets/cloudinit.yml.in")
  vars = {
    admin_user        = var.admin_user
    admin_ssh_pubkeys = jsonencode(var.admin_ssh_pubkeys)
  }
}
