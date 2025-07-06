# Rules for exporting KiCad boards to STEP and STL

KICAD_DIR ?= kicad
STEP_OUT ?= 3d/step
STL_OUT ?= 3d/stl

# find pcb files
PCB_FILES := $(shell find $(KICAD_DIR) -type f -name '*.kicad_pcb')

PCB_TO_STEP = $(STEP_OUT)/board_$(notdir $(basename $(1))).step
PCB_STEP_TO_STL = $(STL_OUT)/board_$(notdir $(basename $(1))).stl

STEP_RULES := $(foreach pcb,$(PCB_FILES),$(call PCB_TO_STEP,$(pcb)):$(pcb))
STEP_FILES := $(foreach r,$(STEP_RULES),$(firstword $(subst :, ,$(r))))
PCB_STL_FILES := $(foreach pcb,$(PCB_FILES),$(call PCB_STEP_TO_STL,$(pcb)))

ALL_TARGETS += $(STEP_FILES) $(PCB_STL_FILES)
CLEAN_DIRS  += $(STEP_OUT) $(STL_OUT)

$(foreach r,$(STEP_RULES),\
  $(eval $(firstword $(subst :, ,$(r))): $(lastword $(subst :, ,$(r))))\
)

$(STEP_OUT)/board_%.step:
	@mkdir -p $(STL_OUT)
	@echo "\u27a2\ufe0f  Erzeuge STEP f\xC3\xBCr: $<"
	kicad-cli pcb export step "$<" --output "$@" --grid-origin --include-tracks --include-zones --subst-models
	@echo "\u2705 Exportiert: $@"

$(STL_OUT)/board_%.stl: $(STEP_OUT)/board_%.step
	@mkdir -p $(STL_OUT)
	@echo "\u27a2\ufe0f  Konvertiere PCB STEP zu STL: $<"
	freecad -c "import FreeCAD, Part, Mesh; shape = Part.Shape(); shape.read('$<'); doc = FreeCAD.newDocument('PCB'); pf = doc.addObject('Part::Feature', 'PCB'); pf.Shape = shape; Mesh.export([pf], '$@');" > /dev/null
	@echo "\u2705 Konvertiert: $@"

pcbstl: $(PCB_STL_FILES)
.PHONY: pcbstl
