# Load Ansible variables from S3-stored YAML/JSON

Ansible vars plugin [s3vars](./vars_plugins/s3vars.py). It loads variables
(common variables, host variables and group variables) from YAML and/or JSON
stored in an S3 bucket.

Usage. Set these environment variables (AWS keys and bucket info must be set to
ones that you have a control of):

```sh
AWS_ACCESS_KEY_ID=
AWS_SECRET_ACCESS_KEY=
ANSIBLE_S3VARS_BUCKET=
ANSIBLE_S3VARS_REGION=
ANSIBLE_S3VARS_CONFIG=s3vars/dev.yml
```

Upload [samples/base.yml](./samples/base.yml) into the bucket as key:
`dev/base.yml` and run a playbook. It will show variables set for four hosts.

```console
$ ansible-playbook main.yml
...
ok: [argon] => {
    "msg": "https://example.com/v1 | be818eeafe22df11ed6e382b4b99df30"
}
ok: [xenon] => {
    "msg": "https://example.com/v1 | be818eeafe22df11ed6e382b4b99df30"
}
ok: [boron] => {
    "msg": "https://example.com/v1 | be818eeafe22df11ed6e382b4b99df30"
}
ok: [carbon] => {
    "msg": "https://example.com/v1 | be818eeafe22df11ed6e382b4b99df30"
}
...
```

Then, upload [samples/update.json](./samples/update.json) as `dev/update.json`
and run the playbook again. You'll see updated variables.

```console
$ ansible-playbook main.yml
...
ok: [argon] => {
    "msg": "https://example.com/v1 | be818eeafe22df11ed6e382b4b99df30"
}
ok: [xenon] => {
    "msg": "https://example.com/v1 | X-66326cd94ff95985bea647ba6a57c83f"
}
ok: [boron] => {
    "msg": "https://example.com/v2 | be818eeafe22df11ed6e382b4b99df30"
}
ok: [carbon] => {
    "msg": "https://example.com/v2 | C-136e143ccabaa097a10e806b1d9476ab"
}
...
```

The structure of vars YAML/JSON file is as follows:

```yaml
vars:
  var: content

host_vars:
  hostname:
    var: content

group_vars:
  groupname:
    var: content
```

`vars` sets variables used for all hosts (effectively a shorthand for `all`
group vars). Precedence is: `host_vars` over `group_vars` over `vars`.
