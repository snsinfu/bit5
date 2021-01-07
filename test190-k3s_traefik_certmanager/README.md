# K3s Traefik v1 Let's Encrypt using cert-manager

Modify terraform.tfvars to correct zone and domain name for your Cloudflare
account:

```hcl
zone_name = "your.zone"
domain = "test190.your.domain"
```

Change hostname in `manifests/myapp.yaml` accordinagly:

```yaml
...
  tls:
    - secretName: myapp-tls
      hosts:
        - myapp.test190.illil.io    # CHANGE THIS
  rules:
    - host: myapp.test190.illil.io  # CHANGE THIS
      http:
...
```

Also change the e-mail address in `manifests/letsencrypt.yaml`. The placeholder
does not work (LE rejects example.com domain).

```yaml
...
  acme:
    email: test@example.com         # CHANGE THIS
...
```

Set environment variables:

```sh
export HCLOUD_TOKEN=...
export CLOUDFLARE_API_TOKEN=...
```

Deploy:

```console
$ make
```

Check HTTPS connectivity. Curl reports server certificate, which should be a
staging fake certificate issued by LE.

```consolej
$ make check
...
* Server certificate:
*  subject: CN=myapp.test190.illil.io
*  start date: Jan  7 13:03:49 2021 GMT
*  expire date: Apr  7 13:03:49 2021 GMT
*  issuer: CN=Fake LE Intermediate X1
*  SSL certificate verify result: unable to get local issuer certificate (20), continuing anyway.
```

For production server, add ClusterIssuer that uses production LE endpoint:

```
apiVersion: cert-manager.io/v1
kind: ClusterIssuer

metadata:
  name: letsencrypt-production

spec:
  acme:
    email: test@example.com     # CHANGE THIS
    server: https://acme-v02.api.letsencrypt.org/directory
    privateKeySecretRef:
      name: production-issuer-account-key

    solvers:
      - http01:
          ingress:
            class: traefik
```


## References

- https://sysadmins.co.za/https-using-letsencrypt-and-traefik-with-k3s/
- https://cert-manager.io/docs/installation/kubernetes/

