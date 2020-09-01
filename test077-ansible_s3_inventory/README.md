# Load JSON inventory from AWS S3

Create an S3 bucket and an IAM user that can access it ([see the previous post
](../test076-boto/README.md)). Upload hostvars.json to the bucket.

Next, set these environment variables:

```sh
export AWS_DEFAULT_REGION=...
export AWS_ACCESS_KEY_ID=...
export AWS_SECRET_ACCESS_KEY=...
export ANSIBLE_INVENTORY_S3_BUCKET=...
```

Or, alternatively, set the information in `inventory/hostvars_s3.yml`:

```yaml
plugin: s3
region: ...
aws_access_key: ...
aws_secret_key: ...
bucket: ...
key: hostvars.json
```

You may want to encrypt the file with `ansible-vault` if you do this.

Then, run `main.yml` to check if a variable is actually loaded from the
inventory stored in the S3 bucket:

```console
$ ansible-playbook main.yml
...
TASK [Show hostvar read from S3 inventory] ***********************************
ok: [localhost] => {
    "msg": "4BqmFXjIzknsSpNuG8G1ZCIC"
}
...
```
