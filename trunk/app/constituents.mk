libnames := appdata xtcrunset
libsrcs_appdata := XtcMonitorServer.cc XtcMonitorClient.cc XtcMonitorMsg.cc TransitionCache.cc
libsrcs_xtcrunset := XtcRunSet.cc

#tgtnames = xtcreader xtcmonserver xtcmonclient xtcmonclientexample acqconfig agatfile
tgtnames = cfgreader xtcreader ebeamreader livextcreader xtcmonserver xtcmonclient xtcmonclientexample xtcEpicsReaderTest dmgreader bldreader xtcmodify xtcmonwriter oldmonserver xtccompress

#CXXFLAGS += -pthread -m32 -I/reg/g/pcds/package/root/include

#LXFLAGS += -L/reg/g/pcds/package/root/lib -lCore -lCint -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -pthread -lm -ldl -rdynamic

tgtsrcs_xtcreader := xtcreader.cc
tgtlibs_xtcreader := psddl_pdsdata xtcdata 
tgtslib_xtcreader := $(USRLIBDIR)/rt

tgtsrcs_cfgreader := cfgreader.cc
tgtlibs_cfgreader := psddl_pdsdata xtcdata 
tgtslib_cfgreader := $(USRLIBDIR)/rt

tgtsrcs_ebeamreader := ebeamreader.cc
tgtlibs_ebeamreader := psddl_pdsdata xtcdata 
tgtslib_ebeamreader := $(USRLIBDIR)/rt

tgtsrcs_xtcmodify := xtcmodify.cc
tgtlibs_xtcmodify := psddl_pdsdata xtcdata 
tgtslib_xtcmodify := $(USRLIBDIR)/rt

tgtsrcs_livextcreader := livextcreader.cc
tgtlibs_livextcreader := psddl_pdsdata xtcdata 
tgtslib_livextcreader := $(USRLIBDIR)/rt

tgtsrcs_dmgreader := dmgreader.cc
tgtlibs_dmgreader := psddl_pdsdata xtcdata 
tgtslib_dmgreader := $(USRLIBDIR)/rt

tgtsrcs_bldreader := bldreader.cc
tgtlibs_bldreader := psddl_pdsdata xtcdata 
tgtslib_bldreader := $(USRLIBDIR)/rt

tgtsrcs_agatfile := agatfile.cc
tgtlibs_agatfile := psddl_pdsdata xtcdata 
tgtslib_agatfile := $(USRLIBDIR)/rt

tgtsrcs_acqconfig := acqconfig.cc
tgtlibs_acqconfig := psddl_pdsdata xtcdata 

tgtsrcs_xtcmonserver := xtcmonserver.cc
tgtlibs_xtcmonserver := xtcrunset appdata anadata indexdata psddl_pdsdata xtcdata
tgtslib_xtcmonserver := $(USRLIBDIR)/rt

tgtsrcs_oldmonserver := oldmonserver.cc
tgtlibs_oldmonserver := psddl_pdsdata appdata xtcdata 
tgtslib_oldmonserver := $(USRLIBDIR)/rt

tgtsrcs_xtcmonclient := xtcmonclient.cc 
tgtlibs_xtcmonclient := appdata psddl_pdsdata xtcdata
tgtslib_xtcmonclient := $(USRLIBDIR)/rt

tgtsrcs_xtcmonclientexample := xtcMonClientExample.cc
tgtlibs_xtcmonclientexample := appdata psddl_pdsdata xtcdata
tgtslib_xtcmonclientexample := $(USRLIBDIR)/rt

tgtsrcs_agatfile := agatfile.cc
tgtlibs_agatfile := psddl_pdsdata xtcdata
tgtslib_agatfile := $(USRLIBDIR)/rt

tgtsrcs_xtcEpicsReaderTest := xtcEpicsReaderTest.cc XtcEpicsFileReader.cc XtcEpicsFileReader.hh XtcEpicsIterator.cc XtcEpicsIterator.hh
tgtincs_xtcEpicsReaderTest := 
tgtlibs_xtcEpicsReaderTest := psddl_pdsdata xtcdata
tgtslib_xtcEpicsReaderTest := 

tgtsrcs_xtcmonwriter := xtcmonwriter.cc 
tgtlibs_xtcmonwriter := appdata psddl_pdsdata xtcdata
tgtslib_xtcmonwriter := $(USRLIBDIR)/rt

tgtsrcs_xtccompress := xtccompress.cc
tgtlibs_xtccompress := compressdata psddl_pdsdata xtcdata
tgtslib_xtccompress := $(USRLIBDIR)/rt

