# Package level makefile
# ----------------------

# Checks
# ------
# Check release location variables
export RELEASE_DIR := $(PWD)/..

ifdef GENDEVEL
export PARSEDEVEL := -D
endif

ddls     := $(filter-out xtc.ddl, $(notdir $(wildcard $(RELEASE_DIR)/psddldata/data/*.ddl)))
ddl_src  := $(patsubst %.ddl,src/%.ddl.cpp,$(ddls))

libnames := psddl_pdsdata
libsrcs_psddl_pdsdata := $(wildcard src/*.cpp)

.PHONY: gen pre-gen

pre-gen:
	$(quiet)rm -f $(ddl_src)

gen: pre-gen $(ddl_src)

#
#  Machine generate the code
#
src/%.ddl.cpp:
	$(quiet)cd $(RELEASE_DIR) && psddlc -b pdsdata -I data -E pdsdata/psddl $(PARSEDEVEL) -O pdsdata/psddl/src -i pdsdata/psddl -t Pds psddldata/data/$*.ddl && cd $(OLDPWD)

package.mk:;
