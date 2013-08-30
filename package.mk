# Package level makefile
# ----------------------
#Makefile:;

# Symbols
# -------
SHELL := /bin/bash
RM    := rm -f
MV    := mv -f
empty :=
space := $(empty) $(empty)

quiet := @
pkg_name := $(notdir $(shell pwd))

# Defines which directories are being created by this makefile
incdir  := $(INSTALLDIR)/pdsdata/${pkg_name}
libdir  := $(INSTALLDIR)/lib/$(INSTALLARCH)
bindir  := $(INSTALLDIR)/bin/$(INSTALLARCH)
objdir  := /tmp/pdsdata/obj
prod_dirs := $(strip $(bindir) $(libdir) $(incdir))

LIBEXTNS := so
DEFINES  := -fPIC -D_REENTRANT -D__pentium__ -Wall
CC  := gcc
CXX := g++
LD  := g++
LX  := g++


# Procedures
# ----------

# Define some procedures and create (different!) rules for libraries
# and targets. Note that 'eval' needs gmake >= 3.80.
incfiles  := $(wildcard *.h *.hh *.hpp)
libraries :=
targets   :=
objects   := 
getobjects = $(strip \
	$(patsubst %.cc,$(1)/%.o,$(filter %.cc,$(2))) \
	$(patsubst %.cpp,$(1)/%.o,$(filter %.cpp,$(2))) \
	$(patsubst %.c,$(1)/%.o, $(filter %.c,$(2))) \
	$(patsubst %.s,$(1)/%.o, $(filter %.s,$(2))))
getprj = $(word 1,$(subst /, ,$(1)))
getlib = $(word 2,$(subst /, ,$(1)))
#getlib = $(1)
getproject = $(libdir)
#getlibrary = $(call getproject,$(call getprj,$(1)))/lib$(call getlib,$(1)).$(LIBEXTNS)
#getlibraries = $(foreach prjlib,$(1),$(call getlibrary,$(prjlib)))
#getprojects  = $(foreach prjlib,$(1),$(call getprj,$(prjlib)))
#getlinkdirs  = $(addprefix -L, $(sort $(foreach prj,$(call getprojects,$(1)),$(call getproject,$(prj)))))
getlinksdir  = $(addprefix -L, $(sort $(dir $(1))))
getlinklibs  = $(addprefix -l,$(foreach prjlib,$(1),$(call getlib,$(prjlib))))
getlinkslib  = $(addprefix -l,$(notdir $(1)))
#getrpath  = $$ORIGIN/../../../$(1)/lib/$(tgt_arch)
#getrpaths = $(subst $(space),:,$(strip $(foreach prj,$(call getprojects,$(1)),$(call getrpath,$(prj)))))


define library_template
  library_$(1) := $$(libdir)/lib$(1).$(LIBEXTNS)
  libobjs_$(1) := $$(call getobjects,$$(objdir),$$(libsrcs_$(1)))
  libraries    += $$(library_$(1))
  objects      += $$(libobjs_$(1))
#  libraries_$(1) := $$(call getlibraries,$$(liblibs_$(1)))
#  linkdirs_$(1)  := $$(call getlinkdirs,$$(liblibs_$(1)))
#  linkdirs_$(1)  += $$(call getlinksdir,$$(libslib_$(1)))
ifneq ($$(liblibs_$(1)),)
#  linklibs_$(1)  := $$(call reverse,$$(call getlinklibs,$$(liblibs_$(1))))
endif
ifneq ($$(libslib_$(1)),)
#  linklibs_$(1)  += $$(call reverse,$$(call getlinkslib,$$(libslib_$(1))))
endif
ifeq ($$(LIBEXTNS),so)
ifneq ($$(ifversn_$(1)),)
  ifversnflags_$(1) := -Wl,--version-script=$$(ifversn_$(1))
endif
endif
#  linkflags_$(1) := $$(linkdirs_$(1)) $$(linklibs_$(1))
$$(library_$(1)): $$(libobjs_$(1))
endef

$(foreach lib,$(libnames),$(eval $(call library_template,$(lib))))

define target_template
  target_$(1)  := $$(bindir)/$(1)
  tgtobjs_$(1) := $$(call getobjects,$$(objdir),$$(tgtsrcs_$(1)))
  targets      += $$(target_$(1))
  objects      += $$(tgtobjs_$(1))
#  libraries_$(1) := $$(call getlibraries,$$(tgtlibs_$(1)))
#  linkdirs_$(1)  := $$(call getlinkdirs,$$(tgtlibs_$(1)))
  linkdirs_$(1)  := -L$(libdir)
  linkdirs_$(1)  += $$(call getlinksdir,$$(tgtslib_$(1)))
ifneq ($$(tgtlibs_$(1)),)
#  linklibs_$(1)  := $$(call reverse,$$(call getlinklibs,$$(tgtlibs_$(1))))
  linklibs_$(1)  := $$(call getlinklibs,$$(tgtlibs_$(1)))
endif
ifneq ($$(tgtslib_$(1)),)
#  linklibs_$(1)  += $$(call reverse,$$(call getlinkslib,$$(tgtslib_$(1))))
  linklibs_$(1)  += $$(call getlinkslib,$$(tgtslib_$(1)))
endif
ifeq ($$(LIBEXTNS),so)
#  rpaths_$(1)    := -Wl,-rpath='$$(call getrpaths,$$(tgtlibs_$(1)))'
  rpaths_$(1)    := -Wl,-rpath='$(libdir)'
endif
linkflags_$(1) := $$(linkdirs_$(1)) $$(linklibs_$(1)) $$(rpaths_$(1))
$$(target_$(1)): $$(tgtobjs_$(1)) $$(libraries_$(1))
endef

$(foreach tgt,$(tgtnames),$(eval $(call target_template,$(tgt))))

temp_dirs := $(strip $(sort $(foreach o,$(objects),$(dir $(o)))))

# Rules
# -----
rules := all dir obj lib bin clean cleanall userall userclean print

.PHONY: $(rules) $(libnames) $(tgtnames)

.SUFFIXES:  # Kills all implicit rules

all: all-m

all-m: clean $(prod_dirs) $(temp_dirs) bin install clean;

obj: $(objects);

lib: $(libraries);

bin: lib $(targets);

install:
	cp ${incfiles} $(incdir)

print:
	@echo	"bindir    = $(bindir)"
	@echo	"libdir    = $(libdir)"
	@echo	"objdir    = $(objdir)"
	@echo	"prod_dirs = $(prod_dirs)"
	@echo	"temp_dirs = $(temp_dirs)"
	@echo   "targets   = $(targets)"
	@echo	"libraries = $(libraries)"
	@echo	"objects   = $(objects)"

clean: userclean
	$(quiet)$(RM) -r $(temp_dirs)

cleanall: clean userclean
	$(quiet)$(RM) -r $(temp_dirs)


# Directory structure
$(prod_dirs) $(temp_dirs):
	mkdir -p $@


# Libraries
$(libdir)/lib%.$(LIBEXTNS):
	@echo "[LD] Build library $*"
	$(quiet)$(LD) $(CXXFLAGS) -shared $(ifversnflags_$*) $(linkflags_$*) $^ -o $@


# Executables
$(bindir)/%:
	@echo "[LT] Linking target $*"
	$(quiet)$(LX) $(DEFINES) $(tgtobjs_$*) $(linkflags_$*) $(CXXFLAGS) -o $@


# Objects for C++ assembly files
$(objdir)/%.o: %.cc
	@echo "[CX] Compiling $<"
	$(quiet)$(CXX) $(CPPFLAGS) $(DEFINES) $(CXXFLAGS) -c $< -o $@

$(objdir)/%.o: %.cpp
	@echo "[CX] Compiling $<"
	$(quiet)$(CXX) $(CPPFLAGS) $(DEFINES) $(CXXFLAGS) -c $< -o $@
