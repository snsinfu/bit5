# Podman on Debian Buster

Follow [the installation instruction][inst] to install `podman`. The playbook
[provision.yml](provision.yml) automates the procedure. However, `podman` does
not work out of the box on Debian Buster:

```console
$ echo "FROM nginx" > Dockerfile
$ podman build .
cannot clone: Operation not permitted
user namespaces are not enabled in /proc/sys/kernel/unprivileged_userns_clone
Error: cannot re-exec process
```

You need to enable user namespaces:

```console
$ sudo sysctl kernel.unprivileged_userns_clone=1
```

(This could have been done in the playbook with the `sysctl` module.) After
that, you you can use podman as a non-root user:

```console
$ podman build .
STEP 1: FROM nginx
Completed short name "nginx" with unqualified-search registries (origin: /etc/containers/registries.conf)
Getting image source signatures
Copying blob a076a628af6f done
Copying blob d7f36f6fe38f done
Copying blob 7125e4df9063 done
Copying blob f72584a26f32 done
Copying blob 0732ab25fa22 done
Copying config f6d0b4767a done
Writing manifest to image destination
Storing signatures
STEP 2: COMMIT
--> f6d0b4767a6
f6d0b4767a6c466c178bf718f99bea0d3742b26679081e52dbf8e0c7c4c42d74
```

Looks like image cache is stored under the home directory:

```console
$ ls ~/.local/share/containers/storage
cache   mounts        tmp          vfs             vfs-images
libpod  storage.lock  userns.lock  vfs-containers  vfs-layers

$ du -h -d 1 ~/.local/share/containers/storage 2>/dev/null
72K     /home/vagrant/.local/share/containers/storage/libpod
416K    /home/vagrant/.local/share/containers/storage/vfs-layers
12K     /home/vagrant/.local/share/containers/storage/vfs-containers
626M    /home/vagrant/.local/share/containers/storage/vfs
36K     /home/vagrant/.local/share/containers/storage/vfs-images
52K     /home/vagrant/.local/share/containers/storage/cache
4.0K    /home/vagrant/.local/share/containers/storage/tmp
4.0K    /home/vagrant/.local/share/containers/storage/mounts
626M    /home/vagrant/.local/share/containers/storage
```

Docker-compatible commands work:

```console
$ podman run f6d0b4767a6
...

$ podman system prune
...
```

[inst]: https://podman.io/getting-started/installation
