# K3s server/agent setup

Create master and worker machines:

```console
$ vagrant up
```

Check kubernetes nodes:

```console
$ export KUBECONFIG=_k3s.yaml
$ kubectl get nodes -o wide
NAME     STATUS   ROLES                  AGE     VERSION        INTERNAL-IP   ...
master   Ready    control-plane,master   7m24s   v1.20.0+k3s2   172.29.1.10   ...
worker   Ready    <none>                 5m32s   v1.20.0+k3s2   172.29.1.11   ...
```

Our master node works as `master`. Here, hostname of the nodes must be different
for k3s server/agent setup to work.


### Scheduling


Service load balancer seems to be running on both master and worker node.

```console
$ kubectl get pods -A -o wide
```

Though, deploying nginx app with `replica: 2` creates two pods on only the
worker node:

```console
$ kubectl apply -f app.yaml
$ kubectl get pods -n myapp -o wide
NAME                   READY   STATUS    RESTARTS   AGE   IP          NODE     NOMINATED NODE   READINESS GATES
web-795f7f9cdd-dlzgm   1/1     Running   0          58s   10.42.1.3   worker   <none>           <none>
web-795f7f9cdd-hbrg6   1/1     Running   0          4s    10.42.1.4   worker   <none>           <none>
```

Though, svclb-traefik is still running on the master. I expect service load
balancer to terminate on the master node because no external traffic should
hit the master node. Maybe old traefik is still running on the master node?

I restarted the master node and svclb-traefik is still running on the master
node.

```console
$ vagrant reload master
$ kubectl get pods -A -o wide
NAMESPACE     NAME                                      READY   STATUS      RESTARTS   AGE     IP           NODE     NOMINATED NODE   READINESS GATES
kube-system   helm-install-traefik-sxt4q                0/1     Completed   0          21m     10.42.0.4    master   <none>           <none>
kube-system   svclb-traefik-d8qrh                       2/2     Running     0          19m     10.42.1.2    worker   <none>           <none>
myapp         web-795f7f9cdd-dlzgm                      1/1     Running     0          7m34s   10.42.1.3    worker   <none>           <none>
myapp         web-795f7f9cdd-hbrg6                      1/1     Running     0          6m40s   10.42.1.4    worker   <none>           <none>
kube-system   traefik-696b4d5457-m2bfc                  1/1     Running     0          3m56s   10.42.1.5    worker   <none>           <none>
kube-system   metrics-server-86cbb8457f-9vznr           1/1     Running     1          21m     10.42.0.9    master   <none>           <none>
kube-system   local-path-provisioner-7c458769fb-r9v8m   1/1     Running     1          21m     10.42.0.8    master   <none>           <none>
kube-system   coredns-854c77959c-d4wmb                  1/1     Running     1          21m     10.42.0.10   master   <none>           <none>
kube-system   svclb-traefik-bzkw8                       2/2     Running     2          21m     10.42.0.11   master   <none>           <none>
```

Maybe I misunderstand the `NoSchedule` effect in `tolerations` directive:

- https://github.com/k3s-io/k3s/blob/2ea6b16315c093d739c370f8f035ad3fa5eb5d11/manifests/traefik.yaml#L27

I thought it means that traefik won't scheduled on nodes having `master` role.
But in the test cluster it's just scheduled on every node.
