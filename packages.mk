ifneq ($(findstring i386-linux,$(tgt_arch)),)
# List of packages (low level first)
packages := acqiris camera evr opal1k pulnix control xtc epics bld app 
endif
ifneq ($(findstring ppc-rtems-rce,$(tgt_arch)),)
packages := xtc
endif
