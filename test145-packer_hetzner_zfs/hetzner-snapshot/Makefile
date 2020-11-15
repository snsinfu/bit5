ARTIFACTS = \
  _init.ok \
  _server.ok \
  _known_hosts \
  terraform.tfstate \
  terraform.tfstate.backup

SSHFLAGS = \
  -o IdentitiesOnly=yes \
  -o UserKnownHostsFile=_known_hosts

GET_IP = \
  $$(jq -r '.resources[] | select(.type == "hcloud_server").instances[0].attributes.ipv4_address' < terraform.tfstate)


.PHONY: all clean destroy ssh send_image

all: _server.ok
	@:

clean: destroy
	rm -f $(ARTIFACTS)

destroy:
	terraform destroy -auto-approve
	rm -f _server.ok

ssh: _server.ok
	ssh $(SSHFLAGS) root@$(GET_IP)

send_image: _server.ok
	gzip -c ../output-debian/debian.img | ssh $(SSHFLAGS) root@$(GET_IP) "gzip -cd > /dev/sda"

_init.ok:
	terraform init
	touch $@

_server.ok: _init.ok
	terraform apply -auto-approve
	touch $@
