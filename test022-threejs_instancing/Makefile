LIBS = \
  lib/build/three.module.js \
  lib/examples/jsm/controls/TrackballControls.js

VERSION = 0.118.3
CDN_BASE = https://unpkg.com/three@$(VERSION)


.PHONY: serve clean serve

serve: $(LIBS)
	@python3 server.py

clean:
	rm -rf $(LIBS)

lib/build/three.module.js:
	mkdir -p $$(dirname $@)
	curl -fsSL -o $@ $(CDN_BASE)/build/three.module.js

lib/examples/jsm/controls/TrackballControls.js:
	mkdir -p $$(dirname $@)
	curl -fsSL -o $@ $(CDN_BASE)/examples/jsm/controls/TrackballControls.js
