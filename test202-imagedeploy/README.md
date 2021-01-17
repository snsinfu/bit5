# Imagedeploy

Using [imagedeploy][imagedeploy] to build and deploy an image to a k3s cluster.

First, provision a virtual machine (requires VirtualBox and Ansible):

```console
$ vagrant up
```

The machine runs Debian with Docker, private registry and K3s installed. The
`myapp` directory is copied to the machine. SSH into the machine and see:

```console
$ vagrant ssh
$ cd myapp
$ find .
.
./Dockerfile
./main.go
./manifests
./manifests/myapp.yaml
./Deploy.yaml
./go.mod
```

Now, run `imagedeploy` in the myapp directory in the virtual machine:

```console
$ imagedeploy

* Building image...
> docker build -t registry:5000/myapp/web .
Sending build context to Docker daemon   7.68kB
...
* Uploading image...
> docker push registry:5000/myapp/web
Using default tag: latest
...
* Removing image...
> docker image rm registry:5000/myapp/web
Untagged: registry:5000/myapp/web:latest
...

* Applying manifests...
> kubectl apply -f ./manifests -n myapp
namespace/myapp created
deployment.apps/web created
service/web created
Warning: extensions/v1beta1 Ingress is deprecated in v1.14+, unavailable in v1.22+; use networking.k8s.io/v1 Ingress
ingress.extensions/web created

* Restarting resources...
> kubectl rollout restart -n myapp deployment/web
deployment.apps/web restarted
```

So, the image is built, pushed to the private registry, and deployed to the k3s
cluster. Check images and pods created:

```console
$ docker image ls
REPOSITORY   TAG       IMAGE ID       CREATED              SIZE
<none>       <none>    2c36b3de9ec2   About a minute ago   857MB
golang       1.15      5f46b413e8f5   4 days ago           839MB

$ kubectl get pods -n myapp
NAME                  READY   STATUS    RESTARTS   AGE
web-5cfb5cfb5-z85rj   1/1     Running   0          119s
```

Exit from the virtual machine and clean up.

```console
$ exit
$ vagrant destroy -f
```

[imagedeploy]: https://github.com/snsinfu/imagedeploy
