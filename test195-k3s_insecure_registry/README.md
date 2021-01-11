# K3s insecure private registry

Create insecure private registry and k3s cluster:

```console
$ vagrant up
```

Log in to the machine, build an image and push that image to the private
registry (named `registry:5000`):

```console
$ vagrant ssh
$ cd myapp
$ docker build -t registry:5000/myapp/web .
$ docker push registry:5000/myapp/web
$ docker image rm registry:5000/myapp/web
```

Docker push to the insecure registry works because of [daemon.json
](templates/docker-daemon.json) configuration.

Next, deploy a kubernetes app that uses the image we just built (see [the app
manifest](myapp/myapp.yaml)):

```console
$ kubectl apply -f myapp.yaml
namespace/myapp created
deployment.apps/web created
service/web created
Warning: extensions/v1beta1 Ingress is deprecated in v1.14+, unavailable in v1.22+; use networking.k8s.io/v1 Ingress
ingress.extensions/web created
```

Inspect pods:

```console
$ kubectl get pods -n myapp
NAME                   READY   STATUS    RESTARTS   AGE
web-59f7cc8d5b-xsflq   1/1     Running   0          51s
web-59f7cc8d5b-nqf9f   1/1     Running   0          51s

$ kubectl describe pods -n myapp
...
Containers:
  web:
    Container ID:   containerd://82470387cfbff87bf962eac5786fd6d1e9bd66a992873b11b363fd7ab562be81
    Image:          registry:5000/myapp/web
    Image ID:       registry:5000/myapp/web@sha256:13e4551010728646aa7e1b1ac5313e04cf75d051fa441396832fcd6d600b5e71
```

As you see, the image is successfully pulled from the private registry. This
requires [registries.yaml](templates/k3s-registries.yaml) configuration.
