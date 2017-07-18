_list: Makefile.build Build/ToGlobalMakefile.xsl

bin/.build/Makefile: bin/.build/Makefile.expanded
	xsltproc --xinclude -o $@ Build/ToGlobalMakefile.xsl $<

bin/.build/Makefile.expanded: Makefile.build
	xsltproc --xinclude -o $@ Build/ExpandProducts.xsl $<

-include bin/.build/Makefile

.PHONY: clean
clean:
	rm -rf bin/
