ARTIFACTS = \
  terraform.tfstate \
  terraform.tfstate.backup \
  _inventory \
  _known_hosts \
  _k3s.yaml \
  _init.ok \
  _server.ok \
  _provision.ok \
  _https.ok


.PHONY: all clean destroy ssh checkconfig

all: _https.ok
	@:

clean: destroy
	rm -f $(ARTIFACTS)

destroy:
	terraform destroy -auto-approve
	rm -f _server.ok

ssh: _server.ok
	ssh -o UserKnownHostsFile=_known_hosts hetzner@$$(terraform output -raw ip)

checkconfig:
	KUBECONFIG=_k3s.yaml kubectl describe configmap -n kube-system traefik

_server.ok: _init.ok
	terraform apply -auto-approve
	touch $@

_init.ok:
	terraform init
	touch $@

_inventory: _server.ok
	echo "main ansible_user=hetzner ansible_host=$$(terraform output -raw ip)" > $@

_provision.ok: _inventory provision.yaml
	ansible-playbook provision.yaml
	touch $@

_https.ok: _provision.ok
	curl -kv -sS https://sub.$$(terraform output -raw domain)/
