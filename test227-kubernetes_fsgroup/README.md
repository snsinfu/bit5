# Fixup ownership of NFS volume

The ownership (uid:gid) of an `emptyDir` volume can be fixed up by specifying
`fsGroup` security context like this:

```yaml
securityContext:
  runAsUser: 1234
  runAsGroup: 5678
  fsGroup: 5678

containers:
  - name: myapp
    ...
    volumeMounts:
      - name: volume
        mountPath: /data    # chowned by kubelet at startup

volumes:
  - name: volume
    emptyDir: {}
```

It's handled [here][src-emptyDir] by the `SetVolumeOwnership()` function.
Sadly, the same configuration won't work with an `nfs` volume. There is no
`SetVolumeOwnership()` call in [the source code][src-nfs], so we need to
chown by ourselves with the common [initContainer hack][hack].

[src-emptyDir]: https://github.com/kubernetes/kubernetes/blob/v1.20.4/pkg/volume/emptydir/empty_dir.go#L285
[src-nfs]: https://github.com/kubernetes/kubernetes/blob/v1.20.4/pkg/volume/nfs/nfs.go
[hack]: https://stackoverflow.com/a/51195446


## Try it out

Create a k3s server and log in to it:

```console
$ vagrant up
$ vagrant ssh
```

The server exports an NFS share at `/srv/nfs`. Deploy a `myapp` pod--this
pod (actually a deployment) mounts a subtree of the NFS share as a volume. It
runs as a non-root user 1234:5678 using the initContainer hack.

```console
$ kubectl apply -f myapp.yaml
$ kubectl get pods
NAME                     READY   STATUS     RESTARTS   AGE
myapp-55bbc469bb-b22d7   0/1     Init:0/1   0          1s
$ kubectl get pods
NAME                     READY   STATUS    RESTARTS   AGE
myapp-55bbc469bb-b22d7   1/1     Running   0          13s
```

See that the ownership of the volume (`myapp` subtree in the NFS share) is
fixed up to the desired one:

```console
$ ls -la /srv/nfs/myapp
total 12
drwxr-xr-x 2 1234 5678 4096 Feb 21 06:26 .
drwxrwxrwt 3 root root 4096 Feb 21 05:25 ..
-rw-r--r-- 1 1234 5678   29 Feb 21 06:26 now.txt
```

The file on the NFS volume is periodically updated by a process in the `myapp`:

```console
$ tail -f /srv/nfs/myapp/now.txt
Sun Feb 21 06:27:43 UTC 2021
tail: /srv/nfs/myapp/now.txt: file truncated
Sun Feb 21 06:27:48 UTC 2021
tail: /srv/nfs/myapp/now.txt: file truncated
Sun Feb 21 06:27:53 UTC 2021
tail: /srv/nfs/myapp/now.txt: file truncated
Sun Feb 21 06:27:58 UTC 2021
tail: /srv/nfs/myapp/now.txt: file truncated
Sun Feb 21 06:28:03 UTC 2021
...
```
