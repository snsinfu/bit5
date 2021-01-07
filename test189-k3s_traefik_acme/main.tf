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

resource "cloudflare_record" "main" {
  zone_id = var.cloudflare_zone_id
  name    = var.domain
  type    = "A"
  value   = hcloud_server.main.ipv4_address
  proxied = false
}

resource "cloudflare_record" "wildcard" {
  zone_id = var.cloudflare_zone_id
  name    = "*.${var.domain}"
  type    = "CNAME"
  value   = var.domain
  proxied = false
}

output "ip" {
  value = hcloud_server.main.ipv4_address
}

output "domain" {
  value = var.domain
}
