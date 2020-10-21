Deploy k3s and seaweedfs CSI on a Debian Vagrant box:

```console
$ vagrant up
$ vagrant ssh
```

Check that CSI driver is working:

```console
$ sudo kubectl get pods -n kube-system
...
csi-seaweedfs-node-cfcc8                 2/2     Running   0          30m
csi-seaweedfs-controller-0               4/4     Running   0          30m
```

Check PVC and test pod:

```console
$ sudo kubectl get pvc
NAME              STATUS   VOLUME                                     CAPACITY   ACCESS MODES   STORAGECLASS        AGE
test-volume-pvc   Bound    pvc-b63e4e81-cded-46a9-874b-ded0e1c4cd5e   1Gi        RWO            seaweedfs-storage   7m15s
$ sudo kubectl get pods
NAME       READY   STATUS    RESTARTS   AGE
test-pod   1/1     Running   0          6m55s
```

## Caverts

### Filer host

There are `SEAWEEDFS_FILER` env var settings in the CSI manifest. It is the
host:port of the filer server. The host here cannot be localhost! Because k8s
pods use this information to connect to filer.

### Volume is a unique bucket

Looks like SeaweedFS CSI allocates a unique bucket as a "volume" for each PVC:

```console
$ sudo kubectl get pvc
NAME              STATUS   VOLUME                                     CAPACITY   ACCESS MODES   STORAGECLASS        AGE
test-volume-pvc   Bound    pvc-9b50056a-9bb8-487a-8e96-8a97a1889f51   1Gi        RWO            seaweedfs-storage   18m
$ sudo apt install fuse
$ sudo weed mount -dir=/mnt &
$ ls /mnt/buckets
pvc-9b50056a-9bb8-487a-8e96-8a97a1889f51
$ fg
^C
```

Re-deploying PVC and pod creates another bucket. The old one gets removed!

```console
$ sudo kubectl delete -f test.yml
$ sudo kubectl apply -f test.yml
sudo kubectl get pvc
NAME              STATUS   VOLUME                                     CAPACITY   ACCESS MODES   STORAGECLASS        AGE
test-volume-pvc   Bound    pvc-b63e4e81-cded-46a9-874b-ded0e1c4cd5e   1Gi        RWO            seaweedfs-storage   38s
$ ls /mnt/buckets
pvc-b63e4e81-cded-46a9-874b-ded0e1c4cd5e
```

So, to use SeaweedFS CSI volume as a persistent storage, I should create PVC
just once and keep it until it is really no more needed.
