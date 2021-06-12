ARTIFACTS = \
  sample.ast \
  bindings \
  build \
  src \
  binding.gyp \
  Cargo.toml \
  node_modules \
  _node.ok

.PHONY: main clean

main: sample.ast
	@cat sample.ast

clean:
	rm -rf $(ARTIFACTS)

sample.ast: sample.cmds src/grammar.json
	yarn run --silent tree-sitter parse sample.cmds > $@

src/grammar.json: grammar.js _node.ok
	yarn run --silent tree-sitter generate

_node.ok: package.json
	yarn install
	touch $@
