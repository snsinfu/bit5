# k3s: Traefik v2 IngressRoute with Let's Encrypt

```console
$ 
```

Check:

```console
$ vagrant ssh
$ kubectl apply -f sample.yaml
$ curl curl -kvL -H "Host: myapp.example.com" https://10.0.2.15/
...
* Server certificate:
*  subject: CN=TRAEFIK DEFAULT CERT
*  start date: Feb 18 05:12:41 2021 GMT
*  expire date: Feb 18 05:12:41 2022 GMT
*  issuer: CN=TRAEFIK DEFAULT CERT
*  SSL certificate verify result: unable to get local issuer certificate (20), continuing anyway.
...
Hostname: myapp-6c8644bfdb-qqx9v
IP: 127.0.0.1
IP: ::1
IP: 10.42.0.9
IP: fe80::3c3e:63ff:fe25:2f72
RemoteAddr: 10.42.0.8:51012
GET / HTTP/1.1
Host: myapp.example.com
User-Agent: curl/7.64.0
Accept: */*
Accept-Encoding: gzip
X-Forwarded-For: 10.42.0.1
X-Forwarded-Host: myapp.example.com
X-Forwarded-Port: 443
X-Forwarded-Proto: https
X-Forwarded-Server: traefik-cfb7c95c-z8t4g
X-Real-Ip: 10.42.0.1
```

FIXME: Traefik routing works, but Let's Encrypt certificate is not used (see
the issuer CN).
