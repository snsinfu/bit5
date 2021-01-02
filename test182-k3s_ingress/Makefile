VAGRANT = vagrant
KUBECTL = env KUBECONFIG=_k3s.yaml kubectl

TEST_URL = http://172.29.1.10/
TEST_HOST = myapp.example.com

ARTIFACTS = \
  .vagrant \
  _k3s.yaml \
  _box.ok \
  _provision.ok \
  _app.ok


.PHONY: test info clean

test: _app.ok
	curl -fsS -H "Host: $(TEST_HOST)" $(TEST_URL)

info: _app.ok
	$(KUBECTL) describe ingress -n myapp

clean:
	vagrant destroy -f
	rm -rf $(ARTIFACTS)

_app.ok: _provision.ok
	$(KUBECTL) apply -f app.yaml
	touch $@

_provision.ok: _box.ok
	$(VAGRANT) provision
	touch $@

_box.ok:
	$(VAGRANT) up
	touch $@
