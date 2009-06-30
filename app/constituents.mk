tgtnames = xtcreader agatfile

ifneq ($(findstring x86_64-linux,$(tgt_arch)),)
syslibdir := /usr/lib64
else
syslibdir := /usr/lib
endif

tgtsrcs_xtcreader := xtcreader.cc
tgtlibs_xtcreader := pdsdata/xtcdata pdsdata/acqdata
tgtslib_xtcreader := $(syslibdir)/rt

tgtsrcs_agatfile := agatfile.cc
tgtlibs_agatfile := pdsdata/xtcdata pdsdata/acqdata
tgtslib_agatfile := $(syslibdir)/rt
