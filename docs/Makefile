SPHINXBUILD := sphinx-build
SOURCEDIR   := source
BUILDDIR    := build

.PHONY: Makefile docs

help:
	@echo "all   : build"
	@echo "clean : clean-up"

all:
	$(SPHINXBUILD) $(SOURCEDIR) $(BUILDDIR)

clean:
	$(RM) -r $(BUILDDIR)
