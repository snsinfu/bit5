# Docker registry

Set up private registry (at 172.29.1.10:5000):

```console
$ vagrant up
```

Configure created registry as an insecure registry. Add the following entry to
the Docker daemon configuration. If you use Docker Desktop, open Preferences
and edit config text in the Docker Engine pane, apply and restart.

```json
{
    "insecure-registries": [
        "172.29.1.10:5000"
    ]
}
```

Create an image and push to the private registry.

```console
$ cd image1
$ docker build -t 172.29.1.10:5000/test184-image1 .
$ docker push 172.29.1.10:5000/test184-image1
```

Pull the image and create an another image.

```console
$ cd image2
$ docker build -t 172.29.1.10:5000/test184-image2 .
$ docker push 172.29.1.10:5000/test184-image2
```

Check disk usage:

```console
$ vagrant ssh
$ du -sh /var/lib/docker-registry
27M     /var/lib/docker-registry
```

Clean up everything.

```console
$ vagrant destroy -f
$ docker rmi 172.29.1.10:5000/test184-image1
$ docker rmi 172.29.1.10:5000/test184-image2
```

Be sure to remove the private registry from the `insecure-registries` entry
in your Docker daemon configuration.


## Notes

- htpasswd is in [the standard format][htpasswd]
- Bcrypt requires a random salt in B64 (not Base64) encoding. The salt must
  be 22-characters long and the padding bits must all be zero. In other words,
  it's a string of 22 characters in charset `[./A-Za-z0-9]` with the trailing
  character restricted to subset `[./AB]`.
- Docker calls a registry `insecure` if it is HTTP-only (no HTTPS) and disables
  Basic authentication.
- The registry takes up 27MB of storage after pushing two thin images. 26MB is
  occupied by a single blob. I'm not entirely sure but maybe the compressed
  base image (debian:buster-slim) is stored.

[htpasswd]: https://en.wikipedia.org/wiki/.htpasswd
