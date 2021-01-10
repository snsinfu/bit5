# K3s server/agent setup

## Traefik

Set up a cluster:

```console
$ vagrant up
```

We set label `svccontroller.k3s.cattle.io/enablelb=true` on the worker node so
that service load balancer is *not* enabled on the master node. After initial
setup, `svclb-traefik` is indeed running only on the worker node:

```console
$ export KUBECONFIG=_k3s.yaml
$ scripts/pods.sh -A
NAME                                      NODE     IP
coredns-854c77959c-ftct6                  master   10.42.0.4
metrics-server-86cbb8457f-jggj5           master   10.42.0.3
local-path-provisioner-7c458769fb-ltxsx   master   10.42.0.2
helm-install-traefik-lk2tc                master   10.42.0.5
traefik-6f9cbd9bd4-94mmg                  master   10.42.0.6
svclb-traefik-7kcqg                       worker   10.42.1.2
```

Though, `traefik` is still running on the master node. Because k3s' deployment
tolerates master taint:

```
$ kubectl describe -n kube-system pod -l app=traefik
...
Tolerations:     CriticalAddonsOnly op=Exists
                 node-role.kubernetes.io/control-plane:NoSchedule op=Exists
                 node-role.kubernetes.io/master:NoSchedule op=Exists
                 node.kubernetes.io/not-ready:NoExecute op=Exists for 300s
                 node.kubernetes.io/unreachable:NoExecute op=Exists for 300s
...
```

You can migrate the pod to the worker node by restarting the deployment:

```console
$ kubectl rollout restart -n kube-system deployment/traefik
deployment.apps/traefik restarted

$ scripts/pods.sh -A
NAME                                      NODE     IP
coredns-854c77959c-ftct6                  master   10.42.0.4
metrics-server-86cbb8457f-jggj5           master   10.42.0.3
local-path-provisioner-7c458769fb-ltxsx   master   10.42.0.2
helm-install-traefik-lk2tc                master   10.42.0.5
traefik-6f9cbd9bd4-94mmg                  master   10.42.0.6
svclb-traefik-7kcqg                       worker   10.42.1.2
traefik-5579cd6c76-l7c9g                  worker   <none>

$ scripts/pods.sh -A
NAME                                      NODE     IP
coredns-854c77959c-ftct6                  master   10.42.0.4
metrics-server-86cbb8457f-jggj5           master   10.42.0.3
local-path-provisioner-7c458769fb-ltxsx   master   10.42.0.2
helm-install-traefik-lk2tc                master   10.42.0.5
svclb-traefik-7kcqg                       worker   10.42.1.2
traefik-5579cd6c76-l7c9g                  worker   10.42.1.3
```

Now, both `traefik` and `svclb-traefik` are running on the worker node.


## Cert-manager

How about cert-manager?

```console
$ kubectl apply -f https://github.com/jetstack/cert-manager/releases/download/v1.1.0/cert-manager.yaml
...

$ scripts/pods.sh -n cert-manager
NAME                                      NODE     IP
cert-manager-cainjector-bd5f9c764-d9zq5   master   10.42.0.8
cert-manager-5597cff495-jb9j4             worker   10.42.1.4
cert-manager-webhook-5f57f59fbc-74k7h     master   10.42.0.9
```

So... cainector and webhook pods run on the master node. But, these pods do not
have tolerations against master taint. How?

```console
$ kubectl describe -n cert-manager pods
...
Tolerations:     node.kubernetes.io/not-ready:NoExecute op=Exists for 300s
                 node.kubernetes.io/unreachable:NoExecute op=Exists for 300s
...
```

Well, it turns out, the master node does not have taints:

```console
$ kubectl describe node/master
Taints:             <none>
```

If we add a taint to the master node and redeploy cert-manager, all pods get
migrated to the worker node as expected:

```console
$ kubectl taint node master node-role.kubernetes.io/master:NoSchedule
$ kubectl rollout restart -n cert-manager deployment/cert-manager-{cainjector,webhook}
$ scripts/pods.sh -n cert-manager
NAME                                       NODE     IP
cert-manager-5597cff495-jb9j4              worker   10.42.1.4
cert-manager-cainjector-57757c499f-v7w7x   worker   10.42.1.5
cert-manager-webhook-b5d47865d-kgdh9       worker   10.42.1.6
```

I'm still uncertain if the taint could be applied to the master node when
installing k3s server (by passing --node-taint option). Traefik tolerates
master taint, so I expect it would work, but it needs an experiment.

→ See the following issue and PR:

- https://github.com/k3s-io/k3s/issues/1401
- https://github.com/k3s-io/k3s/pull/1275

So, necessary tolerations are already set. It should work.


## CritialAddonsOnly

The `master` toleration will soon be renamed to `control-plane`:

- https://github.com/kubernetes/kubeadm/issues/2200
- https://github.com/kubernetes/enhancements/blob/master/keps/sig-cluster-lifecycle/kubeadm/2067-rename-master-label-taint/README.md

Also, k3s documentation suggests to use `CriticalAddonsOnly=true:NoExecute`
taint. Indeed, the k3s PR above also adds tolerations against this taint.

- https://rancher.com/docs/k3s/latest/en/installation/ha/ (search `node-taint`
  in the page)

So, as for k3s, we would better use the `CriticalAddonsOnly` taint. Though,
it would also be beneficial to set standard `master` or `control-plane` taint
for non-bundled critical components to be added in the future.

Anyway, I added taints to k3s server configuration. It allowed automatic
migration of `traefik` pod to the worker node without manual restarting.
Great!

```console
$ scripts/pods.sh -A
NAME                                      NODE     IP
local-path-provisioner-7c458769fb-7cphj   master   10.42.0.3
metrics-server-86cbb8457f-m55bc           master   10.42.0.4
coredns-854c77959c-wj9lv                  master   10.42.0.2
helm-install-traefik-kq58g                worker   10.42.1.2
svclb-traefik-64lmx                       worker   10.42.1.4
traefik-6f9cbd9bd4-vm9fl                  worker   10.42.1.3
```

Of course, cert-manager is scheduled to the worker node:

```console
$ kubectl apply -f https://github.com/jetstack/cert-manager/releases/download/v1.1.0/cert-manager.yaml
...
$ scripts/pods.sh -n cert-manager
NAME                                      NODE     IP
cert-manager-cainjector-bd5f9c764-4sdfg   worker   10.42.1.5
cert-manager-5597cff495-2f757             worker   10.42.1.7
cert-manager-webhook-5f57f59fbc-hg7pn     worker   10.42.1.6
```

I'm not yet sure if cert-manager is so critical that it should be scheduled on
the master node.
