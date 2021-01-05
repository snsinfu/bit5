# K3s + gitkube

We install [gitkube][gitkube] on a k3s cluster inside Vagrant. The gitkube ssh
is exposed via k3s' built-in load balancer with port mapped to 5022. We then
deploy [myapp skeleton](manifests/myapp.yaml) and [a Remote](./remote.yaml)
with a sample ssh key.

The gitkube-monitored remote repository should be accessible using the sample
ssh key but I got pubkey error:

```console
$ git remote -v
gitkube ssh://myapp-gitkube@172.29.1.10:5022/~/git/myapp-gitkube (fetch)
gitkube ssh://myapp-gitkube@172.29.1.10:5022/~/git/myapp-gitkube (push)
origin  git@github.com:snsinfu/bit5.git (fetch)
origin  git@github.com:snsinfu/bit5.git (push)

$ git push gitkube HEAD
myapp-gitkube@172.29.1.10: Permission denied (publickey).
fatal: Could not read from remote repository.

Please make sure you have the correct access rights
and the repository exists.
```

I do have configured my ~/.ssh/config and ssh is using the sample key:

```console
$ ssh -vvv -p 5022 myapp-gitkube@172.29.1.10
...
debug1: Offering public key: ...redacted.../bit5/test183-k3s_gitkube/sample_key ED25519 SHA256:vcbYICAbRK0YkDFciC26pshgu8/2emjN267anQXFU4E explicit
debug3: send packet: type 50
debug2: we sent a publickey packet, wait for reply
debug3: receive packet: type 51
debug1: Authentications that can continue: publickey
debug2: we did not send a packet, disable method
debug1: No more authentication methods to try.
```

But it fails...


[gitkube]: https://github.com/hasura/gitkube
