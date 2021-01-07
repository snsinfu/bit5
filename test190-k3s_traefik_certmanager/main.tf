# SERVER ---------------------------------------------------------------------

resource "hcloud_server" "main" {
  name        = "debian"
  location    = "fsn1"
  server_type = "cx11"
  image       = "debian-10"
  ssh_keys    = data.hcloud_ssh_keys.all.ssh_keys.*.name
  user_data   = file("${path.module}/cloudinit.yaml")
}

data "hcloud_ssh_keys" "all" {
}


# DOMAIN ---------------------------------------------------------------------

resource "cloudflare_record" "main" {
  zone_id = local.zone_id
  name    = var.domain
  type    = "A"
  value   = hcloud_server.main.ipv4_address
  proxied = false
}

resource "cloudflare_record" "wildcard" {
  zone_id = local.zone_id
  name    = "*.${var.domain}"
  type    = "CNAME"
  value   = var.domain
  proxied = false
}

locals {
  zone_id = lookup(data.cloudflare_zones.main.zones[0], "id")
}

data "cloudflare_zones" "main" {
  filter {
    name = var.zone_name
  }
}


# OUTPUT ---------------------------------------------------------------------

data "template_file" "inventory" {
  template = file("${path.module}/inventory.tpl")
  vars = {
    server_address = hcloud_server.main.ipv4_address
  }
}

output "ip" {
  value = hcloud_server.main.ipv4_address
}

output "domain" {
  value = var.domain
}

output "inventory" {
  value = data.template_file.inventory.rendered
}
