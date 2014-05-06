bin/build/Makefile: Makefile.build
	xsltproc -o $@ Build/ToGlobalMakefile.xsl $<
-include bin/build/Makefile

PHONY: clean
clean:
	rm -rf bin/
