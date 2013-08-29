include packages.mk

SHELL := /bin/bash

packages.d := $(addsuffix .D,$(packages))

.PHONY: all print $(packages.d)

all: $(packages.d)

print:
	@echo "CURDIR = $(CURDIR)"
	@echo "packages = $(packages)"
	@echo "packages.d = $(packages.d)"
	@echo "CPPFLAGS = $(CPPFLAGS)"
	@echo "CXXFLAGS = $(CXXFLAGS)"
	@echo "MAKE = $(MAKE)"

define package_template
$(1).D:
	@echo "[PK] Package <$(1)>"
	$(quiet)$(MAKE) -C $(1) all
endef

$(foreach pkg,$(packages),$(eval $(call package_template,$(pkg))))

Makefile:;
packages.mk:;

