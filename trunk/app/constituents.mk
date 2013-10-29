libnames := appdata xtcrunset
libsrcs_appdata := XtcMonitorServer.cc XtcMonitorClient.cc XtcMonitorMsg.cc
libsrcs_xtcrunset := XtcRunSet.cc

#tgtnames = xtcreader xtcmonserver xtcmonclient xtcmonclientexample acqconfig agatfile
tgtnames = cfgreader xtcreader livextcreader xtcmonserver xtcmonclient xtcmonclientexample xtcEpicsReaderTest dmgreader bldreader xtcmodify xtcmonwriter oldmonserver xtccompress

#CXXFLAGS += -pthread -m32 -I/reg/g/pcds/package/root/include

#LXFLAGS += -L/reg/g/pcds/package/root/lib -lCore -lCint -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -pthread -lm -ldl -rdynamic

tgtsrcs_xtcreader := xtcreader.cc
tgtlibs_xtcreader := xtcdata psddl_pdsdata
tgtslib_xtcreader := $(USRLIBDIR)/rt

tgtsrcs_cfgreader := cfgreader.cc
tgtlibs_cfgreader := xtcdata psddl_pdsdata
tgtslib_cfgreader := $(USRLIBDIR)/rt

tgtsrcs_xtcmodify := xtcmodify.cc
tgtlibs_xtcmodify := xtcdata psddl_pdsdata
tgtslib_xtcmodify := $(USRLIBDIR)/rt

tgtsrcs_livextcreader := livextcreader.cc
tgtlibs_livextcreader := xtcdata psddl_pdsdata
tgtslib_livextcreader := $(USRLIBDIR)/rt

tgtsrcs_dmgreader := dmgreader.cc
tgtlibs_dmgreader := xtcdata psddl_pdsdata
tgtslib_dmgreader := $(USRLIBDIR)/rt

tgtsrcs_bldreader := bldreader.cc
tgtlibs_bldreader := xtcdata psddl_pdsdata
tgtslib_bldreader := $(USRLIBDIR)/rt

tgtsrcs_agatfile := agatfile.cc
tgtlibs_agatfile := xtcdata psddl_pdsdata  
tgtslib_agatfile := $(USRLIBDIR)/rt

tgtsrcs_acqconfig := acqconfig.cc
tgtlibs_acqconfig := xtcdata psddl_pdsdata

tgtsrcs_xtcmonserver := xtcmonserver.cc
tgtlibs_xtcmonserver := xtcdata psddl_pdsdata appdata xtcrunset anadata indexdata
tgtslib_xtcmonserver := $(USRLIBDIR)/rt

tgtsrcs_oldmonserver := oldmonserver.cc
tgtlibs_oldmonserver := xtcdata psddl_pdsdata appdata
tgtslib_oldmonserver := $(USRLIBDIR)/rt

tgtsrcs_xtcmonclient := xtcmonclient.cc 
tgtlibs_xtcmonclient := xtcdata psddl_pdsdata appdata 
tgtslib_xtcmonclient := $(USRLIBDIR)/rt

tgtsrcs_xtcmonclientexample := xtcMonClientExample.cc
tgtlibs_xtcmonclientexample := psddl_pdsdata xtcdata appdata
tgtslib_xtcmonclientexample := $(USRLIBDIR)/rt

tgtsrcs_agatfile := agatfile.cc
tgtlibs_agatfile := xtcdata psddl_pdsdata
tgtslib_agatfile := $(USRLIBDIR)/rt

tgtsrcs_xtcEpicsReaderTest := xtcEpicsReaderTest.cc XtcEpicsFileReader.cc XtcEpicsFileReader.hh XtcEpicsIterator.cc XtcEpicsIterator.hh
tgtincs_xtcEpicsReaderTest := 
tgtlibs_xtcEpicsReaderTest := xtcdata psddl_pdsdata
tgtslib_xtcEpicsReaderTest := 

tgtsrcs_xtcmonwriter := xtcmonwriter.cc 
tgtlibs_xtcmonwriter := xtcdata psddl_pdsdata appdata
tgtslib_xtcmonwriter := $(USRLIBDIR)/rt

tgtsrcs_xtccompress := xtccompress.cc
tgtlibs_xtccompress := xtcdata psddl_pdsdata compressdata
tgtslib_xtccompress := $(USRLIBDIR)/rt

