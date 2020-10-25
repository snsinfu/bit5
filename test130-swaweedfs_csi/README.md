# Requesting SeaweedFS volume name from CSI PVC

- https://www.velotio.com/engineering-blog/kubernetes-csi-in-action-explained-with-features-and-use-cases
- https://github.com/kubernetes-csi/external-provisioner

The name of the SeaweedFS bucket created for a PVC is the name parameter passed
in a CreateVolumeRequest message:

- https://github.com/seaweedfs/seaweedfs-csi-driver/blob/master/pkg/driver/controllerserver.go#L26

csi-provisioner generates random name:

- https://github.com/kubernetes-csi/external-provisioner/blob/v2.0.3/pkg/controller/controller.go#L500

So, there seems to be no way to control the name of SeaweedFS volume created
via PVC.


## Running

Set up k3s on Debian and deploy SeaweedFS CSI driver. Deploy a test pod with
persistent volume on SeaweedFS.

```console
$ vagrant up
```

Check CSI:

```console
$ vagrant ssh
$ sudo kubectl get pods -n kube-system
...
csi-seaweedfs-node-4v862                 2/2     Running   0          9m12s
csi-seaweedfs-controller-0               4/4     Running   0          9m12s
```

Check PVC and test pod:

```console
$ sudo kubectl get pvc
$ sudo kubectl get pods
```
