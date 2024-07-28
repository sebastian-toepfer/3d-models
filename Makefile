OPENSCAD=/usr/bin/openscad

BUILDDIR  = generated
SOURCEDIR = designs

STRUCTURE               := $(shell find $(SOURCEDIR) -type d)
SRCFILES                := $(addsuffix /*, $(STRUCTURE))
SRCFILES                := $(wildcard $(SRCFILES))
JSONFILES               := $(filter %json, $(SRCFILES))
PARAMETERFILES          := $(JSONFILES:%.json=%)

OPENSCADFILES           := $(filter %scad, $(SRCFILES))
PARAMETERINIZEDFILES    := $(filter $(PARAMETERFILES), $(OPENSCADFILES))
NONPARAMETERINIZEDFILES := $(filter-out $(PARAMETERINIZEDFILES), $(OPENSCADFILES))
CONFIGS                 := $(foreach file, $(JSONFILES), $(addprefix $(file),$(shell jq -r ".parameterSets | to_entries[] | .key" $(file))))
CONFIGS                 := $(subst json,, $(CONFIGS))
CONFIGS                 := $(addsuffix .stl, $(CONFIGS))


TARGETS                 := $(subst $(SOURCEDIR), $(BUILDDIR), $(NONPARAMETERINIZEDFILES:%.scad=%.scad.stl))
TARGETS                 += $(subst $(SOURCEDIR), $(BUILDDIR), $(CONFIGS))

.PHONY: all clean pull

all: ${TARGETS}

.SECONDEXPANSION:
$(BUILDDIR)/%.stl: $$(filter $$(addprefix $(SOURCEDIR)/, $$(basename $$(basename %))).scad, $(OPENSCADFILES)) $$(filter $$(addprefix $(SOURCEDIR)/, $$(addsuffix .json, $$(basename %))), $(JSONFILES))
	mkdir -p $(dir $(@))
	mkdir -p $(BUILDDIR)/new_stls
	$(OPENSCAD) --hardwarnings -m make -D \$$fn=128 $(addprefix -p ,$(filter %json, $(^))) $(addprefix -P ,$(shell echo $(@) | rev | cut -d. -f2 | rev)) -o $@ $(filter-out %json, $(^))
	cp $@ $(BUILDDIR)/new_stls/

pull:
	git pull -r

clean:
	-rm -f ${TARGETS}

