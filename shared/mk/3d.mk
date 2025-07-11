# Rules for generating printable files from OpenSCAD sources
STL_OUT ?= 3d/stl
3MF_OUT ?= 3d/3mf
SCAD_PARTS ?= 3d/openscad/parts
SCAD_FILTER_OUT += %.template.scad

SCAD_PART_FILES := $(wildcard $(SCAD_PARTS)/*.scad)
SCAD_PART_FILES := $(filter-out $(SCAD_FILTER_OUT),$(SCAD_PART_FILES))

SCAD_TO_STL = $(STL_OUT)/$(basename $(notdir $(1))).stl
SCAD_TO_3MF = $(3MF_OUT)/$(basename $(notdir $(1))).3mf

PRINTABLE_STL_FILES := \
$(foreach s,$(SCAD_PART_FILES),$(call SCAD_TO_STL,$(s)))
PRINTABLE_3MF_FILES := \
$(foreach s,$(SCAD_PART_FILES),$(call SCAD_TO_3MF,$(s)))

ALL_TARGETS += stl 3mf
CLEAN_DIRS  += $(STL_OUT) $(3MF_OUT)

$(STL_OUT)/%.stl: $(SCAD_PARTS)/%.scad
	@mkdir -p $(STL_OUT)
	@echo "\u27a2\ufe0f  Erzeuge STL aus OpenSCAD: $<"
	openscad --hardwarnings -m render -D \$$fn=128 "$<" -o "$@"
	@echo "\u2705 Konvertiert: $@"

$(3MF_OUT)/%.3mf: $(SCAD_PARTS)/%.scad
	@mkdir -p $(3MF_OUT)
	@echo "\u27a2\ufe0f  Erzeuge 3MF aus OpenSCAD: $<"
	openscad --hardwarnings -m render -D \$$fn=128 "$<" -o "$@"
	@echo "\u2705 Konvertiert: $@"

stl: $(PRINTABLE_STL_FILES)
3mf: $(PRINTABLE_3MF_FILES)

.PHONY: stl 3mf

