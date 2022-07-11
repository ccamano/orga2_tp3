BUILDDIR = build

ECHO_GREEN = "\e[01;32m"

.PHONY: all clean image

all: clean image

image:
	make -C src
	@echo $(ECHO_GREEN)'OK'

clean: | $(BUILDDIR)
	@echo 'Limpiando todo...'
	rm -f $(BUILDDIR)/*
	@echo ''

$(BUILDDIR):
	@mkdir -p $(BUILDDIR)

