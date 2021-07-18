GTEX_BASE = https://storage.googleapis.com/gtex_analysis_v8/rna_seq_data
GTEX_FILE = GTEx_Analysis_2017-06-05_v8_RNASeQCv1.1.9_gene_tpm.gct.gz
TARGETS = adjacency.txt


.PHONY: all clean

all: $(TARGETS)
	@:

adjacency.txt: GTEx.h5
	python adjacency.py '$<' | tee '$@'

GTEx.h5: $(GTEX_FILE)
	gzip -cd '$<' | gct_to_hdf5/main '$@'

GTEx_%.gct.gz:
	curl -fsSL -o '$@' '$(GTEX_BASE)/$(notdir $@)'
