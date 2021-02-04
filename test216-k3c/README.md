# K3c

```console
$ vagrant ssh
$ k3c build -t localhost/myapp/main:latest .
$ kubectl apply -f manifest.yaml
$ kubectl get pods -n myapp
$ kubectl exec -n myapp main -it -- /bin/bash
```

- K3c does not tag image with `latest` by default. You need to explicitly tag
  image. Otherwise no tag is set (while kubelet assumes `latest` if no tag is
  specified).
- Image pull policy needs to be `Never` or at least `IfNotPresent`. Default
  policy allows kubelet to try to pull image from docker.io.
