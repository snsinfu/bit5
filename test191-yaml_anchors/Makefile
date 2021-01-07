.PHONY: check

check:
	yq e manifest.yaml -j | yq e '.. style=""' -
