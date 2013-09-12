# Rules
# -----
.PHONY: $(archs)

define arch_template
$(1):
	@echo "Make arch template <$(1)>"
	$$(quiet)$$(MAKE) PREMAKE_DONE=y tgt_arch=$(1) all
endef

$(foreach arc,$(archs),$(eval $(call arch_template,$(arc))))

premake.mk:;