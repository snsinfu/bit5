# K3s server/agent setup

Create master and worker machines:

```console
$ vagrant up
```

Check kubernetes nodes:

```console
$ export KUBECONFIG=_k3s.yaml
$ kubectl get nodes
NAME     STATUS   ROLES                  AGE     VERSION
buster   Ready    control-plane,master   4m15s   v1.20.0+k3s2
```

FIXME: worker is not recognized. Maybe hostname issue?
