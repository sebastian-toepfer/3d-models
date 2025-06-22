OPENSCAD=/usr/bin/openscad

BUILDDIR  = generated
SOURCEDIR = designs

STRUCTURE               := $(shell find $(SOURCEDIR) -type d)
SRCFILES                := $(addsuffix /*, $(STRUCTURE))
SRCFILES                := $(wildcard $(SRCFILES))
JSONFILES               := $(filter %json, $(SRCFILES))
PARAMETERFILES          := $(JSONFILES:%.json=%.scad)

OPENSCADFILES           := $(filter %scad, $(SRCFILES))
PARAMETERINIZEDFILES    := $(filter $(PARAMETERFILES), $(OPENSCADFILES))
NONPARAMETERINIZEDFILES := $(filter-out $(PARAMETERINIZEDFILES), $(OPENSCADFILES))
CONFIGS                 := $(foreach file, $(JSONFILES), $(addprefix $(file),$(shell jq -r ".parameterSets | to_entries[] | .key" $(file))))
CONFIGS                 := $(subst json,, $(CONFIGS))

STL_TARGETS             := $(subst $(SOURCEDIR), $(BUILDDIR), $(NONPARAMETERINIZEDFILES:%.scad=%.stl))
STL_TARGETS             += $(subst $(SOURCEDIR), $(BUILDDIR), $(addsuffix .stl, $(CONFIGS)))

3MF_TARGETS             := $(subst $(SOURCEDIR), $(BUILDDIR), $(NONPARAMETERINIZEDFILES:%.scad=%.3mf))
3MF_TARGETS             += $(subst $(SOURCEDIR), $(BUILDDIR), $(addsuffix .3mf, $(CONFIGS)))

.PHONY: all clean pull

all: ${STL_TARGETS} ${3MF_TARGETS}

.SECONDEXPANSION:
$(BUILDDIR)/%.stl: $$(filter $$(addprefix $(SOURCEDIR)/, $$(basename %)).scad, $(OPENSCADFILES)) $$(filter $$(addprefix $(SOURCEDIR)/, $$(addsuffix .json, $$(basename %))), $(JSONFILES))
	mkdir -p $(dir $(@))
	mkdir -p $(BUILDDIR)/new_stls
	$(OPENSCAD) --hardwarnings -m render -D \$$fn=128 $(addprefix -p ,$(filter %json, $(^))) $(addprefix -P ,$(shell echo $(@) | rev | cut -d. -f2 | rev)) -o $@ $(filter-out %json, $(^))
	cp $@ $(BUILDDIR)/new_stls/

.SECONDEXPANSION:
$(BUILDDIR)/%.3mf: $$(filter $$(addprefix $(SOURCEDIR)/, $$(basename %)).scad, $(OPENSCADFILES)) $$(filter $$(addprefix $(SOURCEDIR)/, $$(addsuffix .json, $$(basename %))), $(JSONFILES))
	mkdir -p $(dir $(@))
	mkdir -p $(BUILDDIR)/new_stls
	$(OPENSCAD) --hardwarnings -m render -D \$$fn=128 $(addprefix -p ,$(filter %json, $(^))) $(addprefix -P ,$(shell echo $(@) | rev | cut -d. -f2 | rev)) -o $@ $(filter-out %json, $(^))
	cp $@ $(BUILDDIR)/new_stls/

pull:
	git pull -r

clean:
	-rm -rf $(BUILDDIR)/new_stls
	-rm -f ${STL_TARGETS}
	-rm -f ${3MF_TARGETS}

