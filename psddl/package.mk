# Package level makefile
# ----------------------

# Checks
# ------
# Check release location variables
export RELEASE_DIR := $(PWD)/..

xmls     := $(filter-out xtc.ddl.xml, $(notdir $(wildcard $(RELEASE_DIR)/psddldata/data/*.ddl.xml)))
xml_src  := $(patsubst %.xml,src/%.cpp,$(xmls))
cpps     := $(notdir $(wildcard $(RELEASE_DIR)/psddl_pdsdata/src/*.cpp))
cpp_src  := $(filter-out $(xml_src), $(patsubst %.cpp,src/%.cpp,$(cpps)))
libnames := psddl_pdsdata
libsrcs_psddl_pdsdata := $(xml_src) $(cpp_src)

.PHONY: gen pre-gen

pre-gen:
	$(quiet)rm -f $(libsrcs_psddl_pdsdata)

gen: pre-gen $(xml_src) $(cpp_src)

#
#  Machine generate the code
#
src/%.ddl.cpp:
	$(quiet)cd $(RELEASE_DIR) && psddlc -b pdsdata -I data -E pdsdata/psddl -O pdsdata/psddl/src -i pdsdata/psddl -t Pds psddldata/data/$*.ddl.xml && cd $(OLDPWD)

#
#  Copy the manual code (replacing PsddlPds namespace with Pds)
#
src/%.cpp:
	$(quiet)sed -e 's/psddl_pdsdata/pdsdata\/psddl/' $(RELEASE_DIR)/psddl_pdsdata/$@ | sed -e 's/PsddlPds/Pds/' > $@

package.mk:;
