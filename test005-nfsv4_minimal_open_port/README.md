# NFSv4 with restrictive firewall

NFSv4 only needs 2049/tcp to be open. No need to open ports for rpcbind or
mountd. That said, the nfsd implementation and the kernel still depends on
mountd daemon to load /etc/exports and update export list.

- NFSv4 server host must run `nfsd` and `mountd`
- Opening 2049/tcp is enough for NFSv4 to work
- `service mountd reload` is required after changing /etc/exports
