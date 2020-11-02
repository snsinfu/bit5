Create k3s/seaweed machine:

```console
$ vagrant up
```

Log in to the machine and create seaweedfs csi volume:

```console
$ vagrant ssh
$ sudo kubectl apply -f volume.yaml
```

Create a test pod app:

```console
$ sudo kubectl apply -f app.yaml
```

Watch changes:

```console
$ /usr/sbin/weed watch
...
```

Execute shell in the test pod. Create some file; e.g., `date > /data/a.txt`.

```console
$ sudo kubectl exec --stdin --tty test-pod -- /bin/sh
```

Re-create pod.

```console
$ sudo kubectl delete -f app.yaml
$ sudo kubectl apply -f app.yaml
```

Verify that the previously created file actually persists.

```console
$ sudo kubectl exec test-pod -- /bin/cat /data/a.txt
```
