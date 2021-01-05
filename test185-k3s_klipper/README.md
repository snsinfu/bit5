# Exposing a TCP port on k3s

K3s comes with builtin [Klipper][klipper] load balancer. So, to expose a TCP
service to the external world, simply create `Service` resource with
`LoadBalancer` type like this:

```yaml
apiVersion: v1
kind: Service

metadata:
  name: some-service

spec:
  type: LoadBalancer
  ports:
  - port: 2222
    targetPort: 22
  selector:
    app: some-deployment
```

With this manifest, the 22/tcp of the pods created by the deployment labeled as
`app=some-deployment` is accessible as `external-ip:2222`.

For HTTP services `Ingress` is much better.


[klipper]: https://github.com/k3s-io/klipper-lb
