libnames := appdata xtcrunset
libsrcs_appdata := XtcMonitorServer.cc XtcMonitorClient.cc XtcMonitorMsg.cc
libsrcs_xtcrunset := XtcRunSet.cc

#tgtnames = xtcreader xtcmonserver xtcmonclient xtcmonclientexample acqconfig agatfile
tgtnames = cfgreader xtcreader livextcreader xtcmonserver xtcmonclient xtcmonclientexample xtcEpicsReaderTest dmgreader bldreader xtcmodify xtcmonwriter oldmonserver xtccompress

#CXXFLAGS += -pthread -m32 -I/reg/g/pcds/package/root/include

#LXFLAGS += -L/reg/g/pcds/package/root/lib -lCore -lCint -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -pthread -lm -ldl -rdynamic

tgtsrcs_xtcreader := xtcreader.cc
tgtlibs_xtcreader := pdsdata/xtcdata pdsdata/psddl_pdsdata pdsdata/aliasdata
tgtslib_xtcreader := $(USRLIBDIR)/rt

tgtsrcs_cfgreader := cfgreader.cc
tgtlibs_cfgreader := pdsdata/xtcdata pdsdata/psddl_pdsdata
tgtslib_cfgreader := $(USRLIBDIR)/rt

tgtsrcs_xtcmodify := xtcmodify.cc
tgtlibs_xtcmodify := pdsdata/xtcdata pdsdata/psddl_pdsdata pdsdata/aliasdata
tgtslib_xtcmodify := $(USRLIBDIR)/rt

tgtsrcs_livextcreader := livextcreader.cc
tgtlibs_livextcreader := pdsdata/xtcdata pdsdata/psddl_pdsdata pdsdata/aliasdata
tgtslib_livextcreader := $(USRLIBDIR)/rt

tgtsrcs_dmgreader := dmgreader.cc
tgtlibs_dmgreader := pdsdata/xtcdata pdsdata/psddl_pdsdata
tgtslib_dmgreader := $(USRLIBDIR)/rt

tgtsrcs_bldreader := bldreader.cc
tgtlibs_bldreader := pdsdata/xtcdata pdsdata/psddl_pdsdata
tgtslib_bldreader := $(USRLIBDIR)/rt

tgtsrcs_agatfile := agatfile.cc
tgtlibs_agatfile := pdsdata/xtcdata pdsdata/psddl_pdsdata  
tgtslib_agatfile := $(USRLIBDIR)/rt

tgtsrcs_acqconfig := acqconfig.cc
tgtlibs_acqconfig := pdsdata/xtcdata pdsdata/psddl_pdsdata

tgtsrcs_xtcmonserver := xtcmonserver.cc
tgtlibs_xtcmonserver := pdsdata/xtcdata pdsdata/psddl_pdsdata pdsdata/appdata pdsdata/xtcrunset pdsdata/anadata pdsdata/indexdata
tgtslib_xtcmonserver := $(USRLIBDIR)/rt

tgtsrcs_oldmonserver := oldmonserver.cc
tgtlibs_oldmonserver := pdsdata/xtcdata pdsdata/psddl_pdsdata pdsdata/appdata
tgtslib_oldmonserver := $(USRLIBDIR)/rt

tgtsrcs_xtcmonclient := xtcmonclient.cc 
tgtlibs_xtcmonclient := pdsdata/xtcdata pdsdata/psddl_pdsdata pdsdata/appdata 
tgtslib_xtcmonclient := $(USRLIBDIR)/rt

tgtsrcs_xtcmonclientexample := xtcMonClientExample.cc
tgtlibs_xtcmonclientexample := pdsdata/psddl_pdsdata pdsdata/xtcdata pdsdata/appdata
tgtslib_xtcmonclientexample := $(USRLIBDIR)/rt

tgtsrcs_agatfile := agatfile.cc
tgtlibs_agatfile := pdsdata/xtcdata pdsdata/psddl_pdsdata
tgtslib_agatfile := $(USRLIBDIR)/rt

tgtsrcs_xtcEpicsReaderTest := xtcEpicsReaderTest.cc XtcEpicsFileReader.cc XtcEpicsFileReader.hh XtcEpicsIterator.cc XtcEpicsIterator.hh
tgtincs_xtcEpicsReaderTest := 
tgtlibs_xtcEpicsReaderTest := pdsdata/xtcdata pdsdata/psddl_pdsdata
tgtslib_xtcEpicsReaderTest := 

tgtsrcs_xtcmonwriter := xtcmonwriter.cc 
tgtlibs_xtcmonwriter := pdsdata/xtcdata pdsdata/psddl_pdsdata pdsdata/appdata
tgtslib_xtcmonwriter := $(USRLIBDIR)/rt

tgtsrcs_xtccompress := xtccompress.cc
tgtlibs_xtccompress := pdsdata/xtcdata pdsdata/psddl_pdsdata pdsdata/compressdata
tgtslib_xtccompress := $(USRLIBDIR)/rt

