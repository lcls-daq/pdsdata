# List of packages (low level first)
ifneq ($(findstring ppc-rtems-rce,$(tgt_arch)),)
packages := xtc
endif

ifneq ($(findstring x86_64-linux,$(tgt_arch)),)
packages := xtc camera phasics timepix
endif

ifneq ($(findstring i386-linux,$(tgt_arch)),)
packages := ipimb encoder pnCCD acqiris camera evr opal1k pulnix control xtc \
            epics bld princeton fccd cspad lusi app xamps ana index fexamp gsc16ai \
            phasics timepix
endif
