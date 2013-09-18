# Package level makefile
# ----------------------

# Checks
# ------
# Check release location variables
export RELEASE_DIR := $(PWD)/..

xmls     := $(filter-out xtc.ddl.xml, $(notdir $(wildcard $(RELEASE_DIR)/psddldata/data/*.ddl.xml)))
xml_src  := $(patsubst %.xml,src/%.cpp,$(xmls))

libnames := psddl_pdsdata
libsrcs_psddl_pdsdata := $(wildcard src/*.cpp)

.PHONY: gen pre-gen

pre-gen:
	$(quiet)rm -f $(xml_src)

gen: pre-gen $(xml_src)

#
#  Machine generate the code
#
src/%.ddl.cpp:
	$(quiet)cd $(RELEASE_DIR) && psddlc -b pdsdata -I data -E pdsdata/psddl -O pdsdata/psddl/src -i pdsdata/psddl -t Pds psddldata/data/$*.ddl.xml && cd $(OLDPWD)

package.mk:;
