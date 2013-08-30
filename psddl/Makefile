# Package level makefile
# ----------------------

# Checks
# ------
# Check release location variables
export RELEASE_DIR := $(PWD)/..

xmls     := $(filter-out xtc.ddl.xml, $(notdir $(wildcard $(RELEASE_DIR)/psddldata/data/*.ddl.xml)))
xml_src  := $(patsubst %.xml,src/%.cpp,$(xmls))
libnames := psddl_pdsdata
libsrcs_psddl_pdsdata := $(xml_src) src/epics.cpp

.PHONY: gen all

#
#  Add machine-generated code step before compilation
#
all: pre-gen gen all-m

pre-gen:
	$(quiet)rm -f $(libsrcs_psddl_pdsdata)
#
#  Handle special manual file (epics.cpp)
#    Replace namespace and include directory and copy into package
#
gen: $(xml_src)
	$(quiet)sed -e 's/psddl_pdsdata/pdsdata\/psddl/' $(RELEASE_DIR)/psddl_pdsdata/src/epics.cpp | sed -e 's/PsddlPds/Pds/' > src/epics.cpp

#
#  Machine generate the code
#
src/%.ddl.cpp:
	$(quiet)cd $(RELEASE_DIR) && psddlc -b pdsdata -I data -E pdsdata/psddl -O pdsdata/psddl/src -i pdsdata/psddl -t Pds psddldata/data/$*.ddl.xml && cd $(OLDPWD)

include ../package.mk

Makefile:;
