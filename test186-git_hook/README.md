# Git hook

```console
$ vagrant up
```

```console
$ cd /tmp
$ git clone ssh://git@172.29.1.10:22/~/myapp
$ cd myapp
$ touch README.md
$ git add README.md
$ git commit
$ git push origin HEAD
...
remote: refs/heads/master: 0000000000000000000000000000000000000000 -> 9ad75a00a193f0821a2a75bab6dce63c45ce575c
...
```

You see the output of the pre-receive script.

TODO: Build image based on Dockerfile, push to local registry and deploy to
k3s cluster.
