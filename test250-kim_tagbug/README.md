# Kim image tag

Provision a k3s + kim system and login:

```console
$ vagrant up
$ vagrant ssh
```

Go to the vagrant-mirrored directory and run test.sh. The script builds two
images with the same tag "localhost/whoami/web:latest".

```console
$ cd /vagrant
$ sh test.sh
 => [internal] load build definition from Dockerfile
 ...
 => exporting to image
 => => exporting layers
 => => exporting manifest sha256:1a7c8403393e0d77548a499aff887bb772acbe68660f2ff5af6e0
 => => exporting config sha256:813616d2279bb84a9cd246312085c1a2b94fa288c8dd8453b403ba1
 => => naming to localhost/whoami/web:latest
[+] Building 0.7s (8/8) FINISHED
 => [internal] load build definition from Dockerfile
 ...
 => exporting to image
 => => exporting layers
 => => exporting manifest sha256:b26d82a973187268af76b976917211f419da39ebb0fdfd3e42886
 => => exporting config sha256:11cb9ad2dda583f036c8df8aa4bdb7dcc9822e7b13c32b45873b9e2
 => => naming to localhost/whoami/web:latest
```

I expect that kim and buildkit replace the old image with the new one. But:

```console
$ kim image ls
IMAGE                            TAG                 IMAGE ID            SIZE
...
localhost/whoami/web:latest      <none>              813616d2279bb       2.58MB
localhost/whoami/web             latest              11cb9ad2dda58       2.58MB
```

The new image (11cb9ad2dda58) is correctly named and tagged. However, the old
image (813616d2279bb) persists with the strange name. It's untagged and
named as "localhost/whoami/web:latest" (this is the name without a tag!). Looks
a bug to me.


## Additional tag

The strange image name does not appear if I attach additional unique tag to
each image.

```console
$ date > date.txt
$ kim build -t localhost/whoami/web:latest -t localhost/whoami/web:$(date +%s) .
$ date > date.txt
$ kim build -t localhost/whoami/web:latest -t localhost/whoami/web:$(date +%s) .
$ kim ls
IMAGE                            TAG                 IMAGE ID            SIZE
...
localhost/whoami/web             1615571731          0395cbd1606c2       2.58MB
localhost/whoami/web             1615571791          42e9ee99555c9       2.58MB
localhost/whoami/web             latest              42e9ee99555c9       2.58MB
```

The first one is the old image, second is the new image and the third is the
alternate name of the second image. Removing the old one by tag actually
removes the image.

```console
$ kim image rm localhost/whoami/web:1615571731
$ kim image ls
IMAGE                            TAG                 IMAGE ID            SIZE
...
localhost/whoami/web             1615571791          42e9ee99555c9       2.58MB
localhost/whoami/web             latest              42e9ee99555c9       2.58MB
```

So, a workaround to the issue is to tag each image with a unique name.


## Build latest image and delete old ones

The following script would do:

```sh
image="localhost/whoami/web"
version="$(date +"%Y%m%d.%H%M%S")"

kim build -t "${image}:${version}" -t "${image}:latest" .

# Identify the ID of the latest image.
latest_id=$(
  kim image ls |
  awk -v image="${image}" '$1 == image && $2 == "latest" { print $3 }'
)

# Delete old images.
kim image ls |
awk -v image="${image}" -v latest_id="${latest_id}" '$1 == image && $3 != latest_id { print $3 }' |
sort -u |
xargs -rL1 kim image rm
```

[Filtering does not work currently][filterbug], so I need to parse the table
and select the target images.

[filterbug]: https://github.com/rancher/kim/blob/v0.1.0-alpha.3/pkg/client/image/list.go#L29

It's cumbersome to repeat this workaround in the deployment script of every
image. Maybe just build an image with a unique tag plus the latest tag:

```sh
# Deployment script.
image="localhost/whoami/web"
version="$(date +"%Y%m%d.%H%M%S")"
kim build -t "${image}:${version}" -t "${image}:latest" .
```

and garbage-collect non-latest images from a cron job. Used images won't be
removed (it's rejected as an error), so it would be okay to just remove (or
attempt to remove) all non-latest images periodically.
