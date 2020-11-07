ARTIFACTS = \
  terraform.tfstate \
  terraform.tfstate.backup \
  _known_hosts \
  _init.ok \
  _server.ok

SSHFLAGS = \
  -o UserKnownHostsFile=_known_hosts \
  -o IdentitiesOnly=yes

IPADDR = \
  $$(jq -r '(.resources[] | select(.type == "hcloud_server")).instances[0].attributes.ipv4_address' < terraform.tfstate)

.PHONY: all clean destroy ssh

all: _server.ok
	@:

clean: destroy
	rm -f $(ARTIFACTS)

destroy:
	terraform destroy -auto-approve
	rm -f _server.ok _known_hosts

ssh:
	ssh $(SSHFLAGS) root@$(IPADDR)

_init.ok:
	terraform init
	touch $@

_server.ok: _init.ok
	terraform apply -auto-approve
	touch $@
