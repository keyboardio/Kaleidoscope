# Minimal makefile for Sphinx documentation
#

# You can set these variables from the command line, and also
# from the environment for the first two.
SPHINXOPTS    ?=
SPHINXBUILD   ?= sphinx-build
SOURCEDIR     = .
BUILDDIR      = ../out/docs

# Put it first so that "make" without argument is like "make help".
help:
	@$(SPHINXBUILD) -M help "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)

.PHONY: help Makefile clean examples

clean:
	rm -rf doxyoutput/ api/
	@$(SPHINXBUILD) -M clean "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)

copy-plugin-readmes:
	-install -d plugins
	(cd ../plugins; \
		ls -d * |xargs -n 1 -I % cp %/README.md ../docs/plugins/%.md )


examples:
	-install -d examples
	(cd ../examples; \
		find . -type f \( -name \*ino -or -name \*h -or -name \*cpp \) \
	) | \
		xargs -n 1 -I % ../bin/docs/example-to-doc %

# Catch-all target: route all unknown targets to Sphinx using the new
# "make mode" option.  $(O) is meant as a shortcut for $(SPHINXOPTS).
%: Makefile 
	@$(SPHINXBUILD) -M $@ "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)

