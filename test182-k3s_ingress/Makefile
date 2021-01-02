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


.PHONY: test clean
.PHONY: apply delete
.PHONY: deployment-info ingress-info

test: _app.ok
	curl -fsS -H "Host: $(TEST_HOST)" $(TEST_URL)

clean:
	vagrant destroy -f
	rm -rf $(ARTIFACTS)

apply: _app.ok
	@:

delete: _provision.ok
	$(KUBECTL) delete -f app.yaml
	rm -f _app.ok

deployment-info: _app.ok
	$(KUBECTL) describe deployment -n myapp

ingress-info: _app.ok
	$(KUBECTL) describe ingress -n myapp


_app.ok: _provision.ok app.yaml
	$(KUBECTL) apply -f app.yaml
	touch $@

_provision.ok: _box.ok provision.yml
	$(VAGRANT) provision
	touch $@

_box.ok:
	$(VAGRANT) up
	touch $@
