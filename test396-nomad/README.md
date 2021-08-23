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
