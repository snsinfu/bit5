KUBECTL = env KUBECONFIG=_k3s.yaml kubectl

ARTIFACTS = \
  .vagrant \
  _k3s.yaml \
  _box.ok \
  _provision.ok \
  _gitkube.ok \
  _myapp.ok \
  _remote.ok


.PHONY: all clean remote check

all: _remote.ok
	@:

clean:
	vagrant destroy -f
	rm -rf $(ARTIFACTS)

check: _remote.ok
	curl -fsS -H "Host: myapp.example.com" http://172.29.1.10/

remote: _remote.ok
	$(KUBECTL) get remote -o yaml -n myapp gitkube | grep ssh://

_remote.ok: _myapp.ok _gitkube.ok
	$(KUBECTL) apply -f remote.yaml
	touch $@

_myapp.ok: _provision.ok
	$(KUBECTL) apply -f manifests/myapp.yaml
	touch $@

_gitkube.ok: _provision.ok
	$(KUBECTL) apply -f gitkube.yaml
	touch $@

_provision.ok: _box.ok provision.yml
	vagrant provision
	touch $@

_box.ok:
	vagrant up --no-provision
	touch $@
