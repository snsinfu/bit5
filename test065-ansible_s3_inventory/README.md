WIP: Read Ansible inventory dynamically from S3 object.

Set these environment variables:

```
AWS_DEFAULT_REGION=
AWS_ACCESS_KEY_ID=
AWS_SECRET_ACCESS_KEY=
ANSIBLE_S3_INVENTORY_BUCKET=
ANSIBLE_S3_INVENTORY_KEY=
```

And run

```console
$ ansible-playbook test.yml
```
