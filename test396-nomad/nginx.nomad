job "demo-nginx" {
  datacenters = ["dc1"]

  group "nginx" {
    network {
      port "web" {
        to = 80  # ?
        static = 8080
      }
    }

    task "nginx" {
      driver = "docker"

      config {
        image = "nginx:1.21"
        ports = ["web"]  # ?
      }

      env {
        NGINX_PORT = "${NOMAD_PORT_http}"
      }
    }
  }
}
