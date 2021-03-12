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
one image (813616d2279bb) persists with the strange name. It's untagged and
named as "localhost/whoami/web:latest" (this is the name without a tag!). Looks
a bug to me.
