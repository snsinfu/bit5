# K3s Traefik v1 Let's Encrypt (failed)

Crete terraform.tfvars:

```hcl
cloudflare_zone_id = "1234..."
domain = "test189.your.domain"
```

Set environment variables:

```sh
export HCLOUD_TOKEN=...
export CLOUDFLARE_API_TOKEN=...
```

Deploy and check curl connectivity:

```console
$ make
...
* Server certificate:
*  subject: C=US; ST=Colorado; L=Boulder; O=ExampleCorp; OU=IT; CN=*.example.com; emailAddress=admin@example.com
*  *  start date: Oct 24 21:09:52 2016 GMT
*  *  expire date: Oct 24 21:09:52 2017 GMT
*  *  issuer: C=US; ST=Colorado; L=Boulder; O=ExampleCorp; OU=IT; CN=*.example.com; emailAddress=admin@example.com
*  *  SSL certificate verify result: self signed certificate (18), continuing anyway.
```

curl succeeds (404 error but HTTPS works). However, Traefik seems to be using
the default self-signed certificate.

```console
$ make checkconfig
...
traefik.toml:
----
# traefik.toml
logLevel = "info"
defaultEntryPoints = ["http","https"]
[entryPoints]
  [entryPoints.http]
  address = ":80"
  compress = true
  [entryPoints.https]
  address = ":443"
  compress = true
    [entryPoints.https.tls]
      [[entryPoints.https.tls.certificates]]
      CertFile = "/ssl/tls.crt"
      KeyFile = "/ssl/tls.key"
  [entryPoints.prometheus]
  address = ":9100"
...
[acme]
KeyType = "RSA4096"
email = "test@example.com"
storage = "/acme/acme.json"
entryPoint = "https"
onHostRule = true
caServer = "https://acme-staging-v02.api.letsencrypt.org/directory"
...
```

Maybe `entryPoints.https.tls.certificates` should not be set?

- https://doc.traefik.io/traefik/v1.7/configuration/acme/
