# NFS CSI CreateVolume

Set up a system:

```console
$ vagrant up
```

The system runs NFSv4 server, dnsmasq (DNS server) and K3s (Kubernetes server).
Now, create a NFS storage class on the Kubernetes cluster for mounting the NFS
volume as a persistent volume. It uses an internal hostname `nfs.internal`
instead of the raw IP address of the NFS server (the hostname is resolved by
the dnsmasq server).

```console
$ kubectl apply -f manifests/storage_class.yaml
```

Create a NFS-backed PV/PVC and a pod that uses the PV as a volume.

```console
$ kubectl apply -f manifests/pod.yaml
```

The PV (NFS) is mounted on the path `/data` in the pod. Let us create a file
in the pod and then check the files from the server side:

```console
$ kubectl exec -n myapp shell -it -- /bin/bash
# date > /data/data.txt
# chown 9999:9999 /data/data.txt
# exit
$ find /srv/nfs
/srv/nfs
/srv/nfs/pvc-623cf49e-bde4-475e-b7a2-772051cb963b
/srv/nfs/pvc-623cf49e-bde4-475e-b7a2-772051cb963b/date.txt
$ ls -hl /srv/nfs/pvc-623cf49e-bde4-475e-b7a2-772051cb963b
total 4.0K
-rw-r--r-- 1 9999 9999 29 Feb  2 16:19 date.txt
```

So, the PVC automatically creates PV as a randomly (?) named directory in the
NFS volume. I tried adding `volumeName` to the PVC in attempt to specify the
name of the created directory but it ended up PVC stuck in pending state. I
think it waited for the PV with the said name to be created externally. I saw
similar behavior when I tried to name Seaweedfs volume...
