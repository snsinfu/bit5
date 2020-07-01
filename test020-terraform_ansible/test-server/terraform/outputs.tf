output "server_addresses" {
  value = {
    test = hcloud_server.test.ipv4_address
  }
}
