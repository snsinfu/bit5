job "demo-whoami" {
  datacenters = ["dc1"]

  group "app" {
    network {
      port "web" {
        to = 80  # port inside the container
      }
    }

    service {
      port = "web"
      tags = [
        "traefik.enable=true",
        "traefik.http.routers.http.rule=Path(`/whoami`)"
      ]
    }

    task "whoami" {
      driver = "docker"

      config {
        image = "containous/whoami:v1.5.0"
        ports = ["web"]
      }

      resources {
        cpu = 100
        memory = 16
      }
    }
  }
}
