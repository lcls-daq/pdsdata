# List of packages (low level first)
ifneq ($(findstring ppc-rtems-rce,$(tgt_arch)),)
packages := xtc
else
packages := ipimb encoder pnCCD acqiris camera evr opal1k pulnix control xtc \
            epics bld princeton fccd cspad lusi xamps fexamp gsc16ai \
            phasics timepix cspad2x2 oceanoptics fli quartz andor usdusb orca \
	    compress ana index app \
	    imp
#
#  Define macro for including all data types
#
datalibs := pdsdata/ipimbdata
datalibs += pdsdata/encoderdata
datalibs += pdsdata/pnccddata
datalibs += pdsdata/acqdata
datalibs += pdsdata/camdata
datalibs += pdsdata/evrdata
datalibs += pdsdata/opal1kdata
datalibs += pdsdata/pulnixdata
datalibs += pdsdata/controldata
datalibs += pdsdata/xtcdata
datalibs += pdsdata/epics
datalibs += pdsdata/bld
datalibs += pdsdata/princetondata
datalibs += pdsdata/fccddata
datalibs += pdsdata/cspaddata
datalibs += pdsdata/lusidata
datalibs += pdsdata/xampsdata
datalibs += pdsdata/fexampdata
datalibs += pdsdata/gsc16aidata
datalibs += pdsdata/phasicsdata
datalibs += pdsdata/timepixdata
datalibs += pdsdata/cspad2x2data
datalibs += pdsdata/oceanopticsdata
datalibs += pdsdata/flidata
datalibs += pdsdata/quartzdata
datalibs += pdsdata/andordata
datalibs += pdsdata/usdusbdata
datalibs += pdsdata/orcadata
datalibs += pdsdata/impdata

endif
