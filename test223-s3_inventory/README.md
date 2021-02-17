# Ansible S3 inventory

Set environment variables:

```console
$ cat .env
AWS_ACCESS_KEY_ID=AK...
AWS_SECRET_ACCESS_KEY=...
AWS_DEFAULT_REGION=...
ANSIBLE_INVENTORY_S3_BUCKET=...
$ dotenv
```

Put test inventory files:

```console
$ s3cmd put -r test s3://$ANSIBLE_INVENTORY_S3_BUCKET
```

Run demo playbook:

```console
$ ansible-playbook main.yml
```

Check some output:

```console
$ cat _hosts
#-6ba9702: Define wireguard hosts
10.20.30.10 ardent.internal
10.20.30.11 beagle.internal
10.20.30.12 cygnet.internal
10.20.30.200 dev1.internal
#-6ba9702
```

Also check ansible host variables:

```console
$ less _vars.yml
...
```

Clean up:

```console
$ s3cmd rm -r s3://$ANSIBLE_INVENTORY_S3_BUCKET/test
```
