# NFS CSI CreateVolume

It is not released yet, but the [NFS CSI driver][nfscsi] now supports
CreteVolume. So, PVC automatically creates a persistent volume (a directory
in the NFS export).

Create NFS + K3s + NFS CSI machine and login:

```console
$ vagrant up
$ vagrant ssh
```

Create NFS storage class:

```console
$ kubectl apply -f storage.yaml
```

Now, any pods in any namespace on the cluster can mount the NFS export through
a PVC.

```console
$ kubectl apply -f myapp.yaml
```

Inspect the PVC:

```console
$ kubectl describe pvc -n myapp storage
...
  Normal  ExternalProvisioning   8m6s (x4 over 8m45s)  persistentvolume-controller                                                             waiting for a volume to be created, either by external provisioner "nfs.csi.k8s.io" or manually created by system administrator
    Normal  Provisioning           7m57s                 nfs.csi.k8s.io_csi-nfs-controller-7fb595656-d92q5_8fdeb8af-a42e-47f3-b691-b5a7d29f580f  External provisioner is provisioning volume for claim "myapp/storage"
      Normal  ProvisioningSucceeded  7m55s                 nfs.csi.k8s.io_csi-nfs-controller-7fb595656-d92q5_8fdeb8af-a42e-47f3-b691-b5a7d29f580f  Successfully provisioned volume pvc-49d1dbd8-25ea-433d-b578-2befa1b34c3a
```

```console
$ kubectl describe pv -n myapp
...
Source:
    Type:              CSI (a Container Storage Interface (CSI) volume source)
    Driver:            nfs.csi.k8s.io
    FSType:
    VolumeHandle:      10.0.2.15//pvc-49d1dbd8-25ea-433d-b578-2befa1b34c3a
    ReadOnly:          false
    VolumeAttributes:      server=10.0.2.15
                           share=/pvc-49d1dbd8-25ea-433d-b578-2befa1b34c3a
                           storage.kubernetes.io/csiProvisionerIdentity=1612198321058-8081-nfs.csi.k8s.io
Events:                <none>
```

Login to the pod and do something on the volume:

```console
$ kubectl exec -n myapp shell -it -- /bin/bash
# date > /data/date.txt
# exit
$ cat /srv/nfs/pvc-*/date.txt
```


[nfscsi]: https://github.com/kubernetes-csi/csi-driver-nfs
