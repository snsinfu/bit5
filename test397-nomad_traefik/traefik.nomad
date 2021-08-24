# FIXME: Job fails

job "traefik" {
  datacenters = ["dc1"]
  type = "service"

  group "app" {
    network {
      port "http" {
        static = 80
      }
    }

    task "traefik" {
      driver = "docker"

      service {
        name = "traefik"
        port = "http"
      }

      config {
        image = "traefik:v2.5"
        network_mode = "host"
        volumes = [
          "local/traefik.yaml:/etc/traefik/traefik.yaml"
        ]
      }

      template {
        data = <<EOF
          entryPoints:
            http:
              address: ":${NOMAD_PORT_http}"
        EOF
        destination = "local/traefik.yaml"
      }

      resources {
        cpu = 100
        memory = 16
      }
    }
  }
}
