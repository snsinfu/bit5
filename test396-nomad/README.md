Create a Nomad VM and login to it:

```
vagrant up
vagrant ssh
```

Check Nomad node status and run demo job:

```
nomad node status
nomad job run nginx.nomad
```

Check IP address of the node:

```
$ nomad node status -verbose
ID                                    DC   Name      Class   Address    Version  Drain  Eligibility  Status
b5bbb220-ced0-d843-1ea9-4754065726c0  dc1  bullseye  <none>  10.0.2.15  1.1.3    false  eligible     ready
```

The port of the nginx service is statically assigned to the port 8080 of the
node. Knowing the IP address of the node, you can now curl the nginx service:

```
curl http://10.0.2.15:8080
```

Though, HTTP services should actually be reverse-proxied via Traefik. Traefik
itself would be served with statically assigned port.
