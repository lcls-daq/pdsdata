libnames := indexdata smalldata

libsrcs_indexdata := src/IndexList.cc src/XtcIterL1Accept.cc src/IndexFileStruct.cc src/IndexFileReader.cc  src/IndexChunkReader.cc  src/IndexSliceReader.cc

libsrcs_smalldata := src/SmlDataList.cc src/SmlDataIterL1Accept.cc src/SmlDataFileStruct.cc src/SmlDataFileReader.cc src/SmlDataChunkReader.cc src/SmlDataSliceReader.cc

tgtnames = xtcindex xtcanalyze xtcanalyzeone smldata

#CXXFLAGS += -pthread -m32 -I/reg/g/pcds/package/root/include

#LXFLAGS += -L/reg/g/pcds/package/root/lib -lCore -lCint -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -pthread -lm -ldl -rdynamic

tgtsrcs_xtcindex := src/xtcindex.cc
tgtlibs_xtcindex := xtcdata psddl_pdsdata indexdata
tgtslib_xtcindex := $(USRLIBDIR)/rt

tgtsrcs_xtcanalyze := src/xtcanalyze.cc
tgtlibs_xtcanalyze := xtcdata psddl_pdsdata indexdata anadata
tgtslib_xtcanalyze := $(USRLIBDIR)/rt

tgtsrcs_xtcanalyzeone := src/xtcanalyzeone.cc
tgtlibs_xtcanalyzeone := xtcdata psddl_pdsdata indexdata
tgtslib_xtcanalyzeone := $(USRLIBDIR)/rt

tgtsrcs_smldata := src/smldata.cc 
tgtlibs_smldata := xtcdata psddl_pdsdata smalldata
tgtslib_smldata := $(USRLIBDIR)/rt


