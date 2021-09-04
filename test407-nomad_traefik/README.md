Running whoami on Nomad/Consul + Traefik. In minimum configuration. Based on
[the officla tutorial](https://learn.hashicorp.com/tutorials/nomad/load-balancing-traefik).

```console
$ vagrant up
$ vagrant ssh

$ cd /vagrant
$ nomad job run traefik.hcl
$ nomad job run whoami.hcl
```


### Traefik

Traefik did not run. Turned out, Nomad did not interpolate `NOMAD_PORT_*`
variable in the Traefik config template. Maybe because I use "static" port
for the task? Anyway, I managed to run Traefik by hard-coding the port number:

```hcl
template {
  data = <<EOF
    entryPoints:
      http:
        address: ":${NOMAD_PORT_http}"  # Bad
        address: ":80"                  # Good
    providers:
      consulCatalog: {}
  EOF
  }
  destination = "local/traefik.yaml"
}
```


### Consul

Consul was not working. Log showed "No cluster leader" errors:

```console
$ sudo journalctl -u consul
...
... [ERROR] agent: Coordinate update error: error="No cluster leader"
... [ERROR] agent.http: Request error: method=GET url=/v1/catalog/services from=127.0.0.1:45302 error="No cluster leader"
```

Following [this SO answer](https://stackoverflow.com/a/58028690), I added
`bootstrap_expect = 1` to Consul configuration (/etc/consul.d/consul-local.hcl).
Consul started to work. Still, I could not access whoami through Traefik:

```console
$ curl http://localhost/whoami   # Traefik
Bad Gateway

$ curl http://10.0.2.15:30656   # Direct
Hostname: f1968d762601
IP: 127.0.0.1
IP: 172.17.0.2
RemoteAddr: 10.0.2.15:36868
GET / HTTP/1.1
Host: 10.0.2.15:30656
User-Agent: curl/7.74.0
Accept: */*
```

Consul recognized the tags. Maybe it did not recognize the container port?

```console
$ consul catalog services -tags
consul
demo-whoami-app      traefik.enable=true,traefik.http.routers.http.rule=Path(`/whoami`)
nomad                http,rpc,serf
nomad-client         http
traefik
```


### Service stanza

I added `port = "web"` to the `service` stanza of the whoami job. It fixed the
"Bad Gateway" issue!

```console
$ curl http://localhost/whoami
Hostname: ce70c6406dd2
IP: 127.0.0.1
IP: 172.17.0.2
RemoteAddr: 10.0.2.15:45450
GET /whoami HTTP/1.1
Host: localhost
User-Agent: curl/7.74.0
Accept: */*
Accept-Encoding: gzip
X-Forwarded-For: ::1
X-Forwarded-Host: localhost
X-Forwarded-Port: 80
X-Forwarded-Proto: http
X-Forwarded-Server: bullseye
X-Real-Ip: ::1
```

So, the service stanza in Nomad job file defines what information to pass to
Consul (thus eventually to Traefik). The `port` key is required for Traefik to
recognize the random port number assigned to a container.
