SRCDIR=src
BUILDDIR=build
OUTDIR=out
VENDORDIR=vendor

UTHASH_VERSION=2.0.2

CFLAGS.base=-std=c99
CFLAGS.errors=-Wall -Werror
CFLAGS.deps=-MD -MF$(patsubst %.o,%.d,$@)
CFLAGS.includes=-I$(VENDORDIR)/uthash-$(UTHASH_VERSION)/src
CFLAGS=$(CFLAGS.base) $(CFLAGS.errors) $(CFLAGS.deps) $(CFLAGS.includes)

# main targets

.PHONY: default
default: $(OUTDIR)/cflakes

$(OUTDIR)/cflakes: $(addprefix $(BUILDDIR)/,cell6.o path.o reiter.o svg.o util.o)


$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(COMPILE.c) $(OUTPUT_OPTION) $<

$(OUTDIR)/%: $(BUILDDIR)/%.o
	$(LINK.c) $^ $(LOADLIBES) $(LDLIBS) -o $@

.PRECIOUS: %.o

# vendor targets

build/path.o: $(VENDORDIR)/uthash-$(UTHASH_VERSION)/src/uthash.h

$(VENDORDIR)/uthash-$(UTHASH_VERSION)/%: $(VENDORDIR)/uthash-$(UTHASH_VERSION).tar.gz
	tar x -C $(VENDORDIR) -zmf $<

$(VENDORDIR)/uthash-$(UTHASH_VERSION).tar.gz:
	curl -Lo $@ https://github.com/troydhanson/uthash/archive/v$(UTHASH_VERSION).tar.gz

# utility targets

.PHONY: clean
clean:
	rm -rf $(BUILDDIR) $(OUTDIR) $(VENDORDIR)

ifneq ($(MAKECMDGOALS),clean)
$(shell mkdir -p $(BUILDDIR) $(OUTDIR) $(VENDORDIR))
endif

-include $(BUILDDIR)/*.d
