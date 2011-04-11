libnames := index
libsrcs_index := src/IndexList.cc src/XtcIterL1Accept.cc src/IndexFileReader.cc src/IndexFileStruct.cc

tgtnames = xtcindex xtcanalyze

#CXXFLAGS += -pthread -m32 -I/reg/g/pcds/package/root/include

#LXFLAGS += -L/reg/g/pcds/package/root/lib -lCore -lCint -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -pthread -lm -ldl -rdynamic

tgtsrcs_xtcindex := src/xtcindex.cc 
tgtlibs_xtcindex := pdsdata/xtcdata pdsdata/acqdata pdsdata/epics pdsdata/pnccddata pdsdata/bld pdsdata/controldata pdsdata/evrdata pdsdata/ipimbdata pdsdata/index
tgtslib_xtcindex := $(USRLIBDIR)/rt

tgtsrcs_xtcanalyze := src/xtcanalyze.cc 
tgtlibs_xtcanalyze := pdsdata/xtcdata pdsdata/acqdata pdsdata/epics pdsdata/pnccddata pdsdata/bld pdsdata/controldata pdsdata/evrdata pdsdata/ipimbdata pdsdata/index
tgtslib_xtcanalyze := $(USRLIBDIR)/rt
