SRCDIR=src
BUILDDIR=build
BINDIR=bin
VENDORDIR=vendor
DOCKERDIR=docker

UTHASH_VERSION=2.0.2

CFLAGS.base=-std=c99
CFLAGS.errors=-Wall -Werror
CFLAGS.deps=-MD -MF$(patsubst %.o,%.d,$@)
CFLAGS.includes=-I$(VENDORDIR)/uthash-$(UTHASH_VERSION)/src
CFLAGS=$(CFLAGS.base) $(CFLAGS.errors) $(CFLAGS.deps) $(CFLAGS.includes)

DOCKER_APP_HOME=/opt/cflakes
DOCKER_SOCK=/var/run/docker.sock
DOCKER_RUN_VOLUMES=-v `pwd`:$(DOCKER_APP_HOME) -v $(DOCKER_SOCK):$(DOCKER_SOCK)
DOCKER_IMAGE_BUILD=cflakes-build
DOCKER_IMAGE_CFLAKES=epilanthanomai/cflakes

# main targets

.PHONY: default
default: $(BINDIR)/cflakes

$(BINDIR)/cflakes: $(addprefix $(BUILDDIR)/,cell6.o path.o reiter.o svg.o util.o)


$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(COMPILE.c) $(OUTPUT_OPTION) $<

$(BINDIR)/%: $(BUILDDIR)/%.o
	$(LINK.c) $^ $(LOADLIBES) $(LDLIBS) -o $@

.PRECIOUS: %.o

# vendor targets

build/path.o: $(VENDORDIR)/uthash-$(UTHASH_VERSION)/src/uthash.h

$(VENDORDIR)/uthash-$(UTHASH_VERSION)/%: $(VENDORDIR)/uthash-$(UTHASH_VERSION).tar.gz
	tar x -C $(VENDORDIR) -zmf $<

$(VENDORDIR)/uthash-$(UTHASH_VERSION).tar.gz:
	curl -Lo $@ https://github.com/troydhanson/uthash/archive/v$(UTHASH_VERSION).tar.gz

# docker targets
.PHONY: docker-cflakes-build
docker-cflakes-build:
	docker build -f $(DOCKERDIR)/build.Dockerfile -t $(DOCKER_IMAGE_BUILD) .

.PHONY: docker-cflakes
docker-cflakes:
	docker run --rm $(DOCKER_RUN_VOLUMES) $(DOCKER_IMAGE_BUILD) sh -c 'cd $(DOCKER_APP_HOME) && make docker-cflakes-nested'

.PHONY: docker-cflakes-nested
docker-cflakes-nested: $(BINDIR)/cflakes
	docker build -f $(DOCKERDIR)/cflakes.Dockerfile -t $(DOCKER_IMAGE_CFLAKES) .

# utility targets

.PHONY: clean
clean:
	rm -rf $(BUILDDIR) $(BINDIR) $(VENDORDIR)

ifneq ($(MAKECMDGOALS),clean)
$(shell mkdir -p $(BUILDDIR) $(BINDIR) $(VENDORDIR))
endif

-include $(BUILDDIR)/*.d
