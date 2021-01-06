# Git hook

Set up git repository, private registry and k3s cluster:

```console
$ vagrant up
```

Create `myapp` repository and push it to the remote repository hosted on the
virtual machine. The pre-receive hook script builds image, push it to the
private registry and deploys kubernetes app to k3s.

```console
$ cp -r myapp /tmp
$ cd /tmp/myapp
$ git init
$ git remote add deploy ssh://git@172.29.1.10:22/~/myapp
$ git add .
$ git commit
$ git push deploy master
Writing objects: 100% (11/11), 1.14 KiB | 1.14 MiB/s, done.
Total 11 (delta 1), reused 0 (delta 0), pack-reused 0
remote: Creating build directory
remote: Checking out master:c30fc49bb6ee603f0a75ba5d4f04ba6d96a5c500
remote: Building image
remote: Sending build context to Docker daemon  4.096kB
remote: Step 1/1 : FROM nginx:1
remote: 1: Pulling from library/nginx
remote: 6ec7b7d162b2: Pulling fs layer
...
remote: Successfully built ae2feff98a0c
remote: Successfully tagged registry:5000/myapp:c30fc49bb6ee603f0a75ba5d4f04ba6d96a5c500
remote: Pushing image
remote: The push refers to repository [registry:5000/myapp]
...
remote: Applying manifests
remote: namespace/myapp created
remote: deployment.apps/main created
remote: service/main created
remote: Warning: extensions/v1beta1 Ingress is deprecated in v1.14+, unavailable in v1.22+; use networking.k8s.io/v1 Ingress
remote: ingress.extensions/main created
remote: Cleaning up build directory
```

FIXME: Pod fails to start due to ErrImagePull. It looks like k3s tries to
access private registry using HTTPS. Configuring `registries.yaml` did not
help.

```
vagrant@buster:~$ kubectl describe pod -n myapp main
Name:         main-76b75554f-2w5jn
Namespace:    myapp
Priority:     0
...
Events:
  Type     Reason     Age                  From               Message
  ----     ------     ----                 ----               -------
  Normal   Scheduled  2m42s                default-scheduler  Successfully assigned myapp/main-76b75554f-2w5jn to buster
  Normal   Pulling    67s (x4 over 2m42s)  kubelet            Pulling image "registry:5000/myapp"
  Warning  Failed     67s (x4 over 2m42s)  kubelet            Error: ErrImagePull
  Warning  Failed     67s (x4 over 2m42s)  kubelet            Failed to pull image "registry:5000/myapp": rpc error: code = Unknown desc = failed to pull and unpack image "registry:5000/myapp:latest": failed to resolve reference "registry:5000/myapp:latest": failed to do request: Head "https://registry:5000/v2/myapp/manifests/latest": http: server gave HTTP response to HTTPS client
  Normal   BackOff    56s (x6 over 2m42s)  kubelet            Back-off pulling image "registry:5000/myapp"
  Warning  Failed     41s (x7 over 2m42s)  kubelet            Error: ImagePullBackOff
```
