KUBECTL = env KUBECONFIG=_k3s.yaml kubectl

ARTIFACTS = \
  .vagrant \
  _k3s.yaml \
  _box.ok \
  _provision.ok \
  _app.ok

TEST_URL = http://172.29.1.10:5000/


.PHONY: test clean

test: _app.ok
	curl -fsS $(TEST_URL)

clean:
	vagrant destroy -f
	rm -rf $(ARTIFACTS)

_app.ok: _provision.ok
	$(KUBECTL) apply -f app.yaml
	touch $@

_provision.ok: _box.ok provision.yml
	vagrant provision
	touch $@

_box.ok:
	vagrant up --no-provision
	touch $@
