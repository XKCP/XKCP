_list: Makefile.build Build/ToGlobalMakefile.xsl

bin/.build/Makefile: bin/.build/Makefile.expanded
	xsltproc -o $@ Build/ToGlobalMakefile.xsl $<

bin/.build/Makefile.expanded: Makefile.build
	xsltproc -o $@ Build/ExpandProducts.xsl $<

-include bin/.build/Makefile

.PHONY: clean
clean:
	rm -rf bin/
