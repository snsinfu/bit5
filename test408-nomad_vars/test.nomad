job "test" {
  datacenters = ["dc1"]

  group "app" {
    network {
      port "http" {
        static = var.http_port
      }
    }

    service {
      name = "caddy"
      port = "http"
    }

    task "caddy" {
      driver = "exec"

      config {
        # XXX: I'm still unsure how exec driver finds out the executable.
        # `caddy` and `local/caddy` works, while `/local/caddy` does not.
        # ps on the host machine shows `/local/caddy` running in chroot.
        command = "caddy"
        args = ["run", "-config", "/local/Caddyfile"]
      }

      template {
        data = <<-EOF
        {
          admin off
        }
        http://:${var.http_port} {
          respond "OK"
        }
        EOF
        destination = "local/Caddyfile"
      }

      # Download caddy to `/local/caddy` in the alloc fs.
      artifact {
        source = var.caddy_release.url
        options {
          checksum = var.caddy_release.checksum
        }
      }

      resources {
        cpu = 100
        memory = 32
      }
    }
  }
}

#
# Variables
#

variable "caddy_release" {
  type = object({
    url = string
    checksum = string
  })
  default = {
    url = "https://github.com/caddyserver/caddy/releases/download/v2.4.5/caddy_2.4.5_linux_amd64.tar.gz"
    checksum = "sha512:1a19a8d94c6c16d8e671c4dad1e835e7d0d586a36e15f5300b374e5d31cf10eafdf07bc2c24d5589cdfaff1dd276e4627917802e858e14dc2351797d7dec7586"
  }
}

variable "http_port" {
  type = number
  default = 8080
  description = "The host port which caddy binds to. This cannot be a privileged port (e.g., 80) since Nomad runs caddy as 'nobody'."

  # You can allow caddy to bind to 80. You then need to remove "root" from
  # `user.denylist` configuration of Nomad client and run caddy as root,
  # which is insecure. Docker would be a better choice for apps that need to
  # bind to privileged ports on the host (like load balancers).
  #
  # https://www.nomadproject.io/docs/configuration/client#user-denylist
}

