ARTIFACTS = \
  terraform.tfstate \
  terraform.tfstate.backup \
  _init.ok \
  _server.ok

.PHONY: all clean destroy

all: _server.ok
	@:

clean: destroy
	rm -f $(ARTIFACTS)

destroy:
	terraform destroy -auto-approve
	@rm -f _server.ok

_init.ok:
	terraform init
	@touch $@

_server.ok: _init.ok
	terraform apply -auto-approve
	@touch $@
