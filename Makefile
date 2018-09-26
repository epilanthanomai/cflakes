SRCDIR=src
BUILDDIR=build
OUTDIR=out

CFLAGS=-Wall -Werror -MD -MF$(patsubst %.o,%.d,$@)


.PHONY: default
default: $(OUTDIR)/cflakes

$(OUTDIR)/cflakes: $(addprefix $(BUILDDIR)/,reiter.o util.o)


$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(COMPILE.c) $(OUTPUT_OPTION) $<

$(OUTDIR)/%: $(BUILDDIR)/%.o
	$(LINK.c) $^ $(LOADLIBES) $(LDLIBS) -o $@

.PRECIOUS: %.o


.PHONY: clean
clean:
	rm -rf $(BUILDDIR) $(OUTDIR)

ifneq ($(MAKECMDGOALS),clean)
$(shell mkdir -p $(BUILDDIR) $(OUTDIR))
endif

-include $(BUILDDIR)/*.d
