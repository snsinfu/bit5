# Terraform and Ansible integration

System global configuration is stored in `config/production.yml`. Ansible loads
that file and executes `instance_up.yml` playbook to construct infrastructure.
The infrastructure configuration is also defined in `config/production.yml` and
passed to Terraform by Ansible (through terraform module). Ansible captures
Terraform output and uses it to update Ansible inventory so that subsequent
execution of `provision.yml` can connect to the created server instance.

```
/
  config/
    production.yml      Infra + software configuration

  test-server/          Single infrastructure group
    terraform/          Normal terraform project
      main.tf
      variables.tf
      outputs.tf

    inventory/          Ansible inventory
      hosts             Static list of hosts
      _10-instance      Inventory fragment containing instance information

    roles/              Ansible roles

    instance_up.yml     Creates infrastructure
    instance_down.yml   Destroys everything
    provision.yml       Configures OS and software
```

## Usage

Create `.env` and `config/production.yml` following examples. Load environment
variables:

```console
$ set -a; . ./.env; set +a
```

Spin up a fresh server:

```console
$ cd test-server
$ ansible-playbook instance_up.yml
$ ansible-playbook provision.yml
```

Destroy the server:

```console
$ ansible-playbook instance_down.yml
```
