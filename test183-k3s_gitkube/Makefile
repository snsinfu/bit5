KUBECTL = env KUBECONFIG=_k3s.yaml kubectl

ARTIFACTS = \
  .vagrant \
  _k3s.yaml \
  _box.ok \
  _provision.ok \
  _gitkube.yaml


.PHONY: all clean

all: _provision.ok
	@:

clean:
	vagrant destroy -f
	rm -rf $(ARTIFACTS)

_provision.ok: _box.ok provision.yml
	vagrant provision
	touch $@

_box.ok:
	vagrant up --no-provision
	touch $@
