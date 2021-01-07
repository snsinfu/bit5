ARTIFACTS = \
  terraform.tfstate \
  terraform.tfstate.backup \
  _inventory \
  _known_hosts \
  _k3s.yaml \
  _init.ok \
  _server.ok \
  _provision.ok \
  _certmanager.ok \
  _letsencrypt.ok \
  _myapp.ok

KUBECTL = env KUBECONFIG=_k3s.yaml kubectl


.PHONY: all clean destroy ssh check

all: _myapp.ok
	@:

clean: destroy
	rm -f $(ARTIFACTS)

destroy:
	terraform destroy -auto-approve
	rm -f _server.ok

ssh: _server.ok
	ssh -o UserKnownHostsFile=_known_hosts hetzner@$$(terraform output -raw ip)

check: _myapp.ok
	curl -kv -sS https://myapp.$$(terraform output -raw domain)/

_myapp.ok: _letsencrypt.ok
	$(KUBECTL) apply -f manifests/myapp.yaml
	@touch $@

_letsencrypt.ok: _certmanager.ok
	$(KUBECTL) apply -f manifests/letsencrypt.yaml
	@touch $@

_certmanager.ok: _provision.ok
	$(KUBECTL) apply -f https://github.com/jetstack/cert-manager/releases/download/v1.1.0/cert-manager.yaml
	@touch $@

_provision.ok: _inventory provision.yaml
	ansible-playbook provision.yaml
	@touch $@

_inventory: _server.ok
	terraform output -raw inventory > $@

_server.ok: _init.ok
	terraform apply -auto-approve
	@touch $@

_init.ok:
	terraform init
	@touch $@
