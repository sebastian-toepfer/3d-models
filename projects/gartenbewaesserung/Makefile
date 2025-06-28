KICAD_DIR := kicad
STEP_OUT := 3d/step
STL_OUT := 3d/stl
3MF_OUT := 3d/3mf
SCAD_PARTS := 3d/openscad/parts
SCAD_MODEL := 3d/openscad/models/gehaeuse.scad
DOCS_DIR := docs/assembly
FRAMES := 1 2 3 4 5
FRAME_FILES := $(addprefix $(DOCS_DIR)/frame_,$(addsuffix .png,$(FRAMES)))
FIRMWARE_DIR := firmware

# Alle .kicad_pcb-Dateien finden
PCB_FILES := $(shell find $(KICAD_DIR) -type f -name '*.kicad_pcb')

# Alle .scad-Dateien in parts finden
SCAD_PART_FILES := $(wildcard $(SCAD_PARTS)/*.scad)

# Hilfsfunktionen
define PCB_TO_STEP
$(STEP_OUT)/board_$(notdir $(basename $(1))).step
endef

define PCB_STEP_TO_STL
$(STL_OUT)/board_$(notdir $(basename $(1))).stl
endef

define SCAD_TO_STL
$(STL_OUT)/$(basename $(notdir $(1))).stl
endef

define SCAD_TO_3MF
$(3MF_OUT)/$(basename $(notdir $(1))).3mf
endef

# Regeln erzeugen
STEP_RULES := $(foreach pcb,$(PCB_FILES),$(call PCB_TO_STEP,$(pcb)):$(pcb))
STEP_FILES := $(foreach rule,$(STEP_RULES),$(firstword $(subst :, ,$(rule))))

PCB_STL_FILES := $(foreach pcb,$(PCB_FILES),$(call PCB_STEP_TO_STL,$(pcb)))

PRINTABLE_STL_FILES := $(foreach scad,$(SCAD_PART_FILES),$(call SCAD_TO_STL,$(scad)))
PRINTABLE_3MF_FILES := $(foreach scad,$(SCAD_PART_FILES),$(call SCAD_TO_3MF,$(scad)))

# Standard-Ziel
all: $(STEP_FILES) $(PCB_STL_FILES) $(PRINTABLE_STL_FILES) $(PRINTABLE_3MF_FILES) frames
	$(MAKE) -C $(FIRMWARE_DIR)

pcbstl: $(PCB_STL_FILES)
3mf: $(PRINTABLE_3MF_FILES)
stl: $(PRINTABLE_STL_FILES)

frames: $(FRAME_FILES)

# Firmware flashen
flash:
	$(MAKE) -C $(FIRMWARE_DIR) upload

# Firmware testen
monitor:
	$(MAKE) -C $(FIRMWARE_DIR) monitor

# Generiere jede Regel explizit
$(foreach rule,$(STEP_RULES), \
  $(eval $(firstword $(subst :, ,$(rule))): $(lastword $(subst :, ,$(rule)))) \
)

# PCB als STEP
$(STEP_OUT)/board_%.step:
	@mkdir -p $(STL_OUT)
	@echo "⭢️  Erzeuge STEP für: $<"
	kicad-cli pcb export step "$<" --output "$@" --grid-origin --include-tracks --include-zones --subst-models
	@echo "✅ Exportiert: $@"

# PCB-STEP zu STL (für die OpenSCAD Verwendung)
$(STL_OUT)/board_%.stl: $(STEP_OUT)/board_%.step
	@mkdir -p $(STL_OUT)
	@echo "⭢️  Konvertiere PCB STEP zu STL: $<"
	freecad -c "import FreeCAD, Part, Mesh; shape = Part.Shape(); shape.read('$<'); doc = FreeCAD.newDocument('PCB'); pf = doc.addObject('Part::Feature', 'PCB'); pf.Shape = shape; Mesh.export([pf], '$@');" > /dev/null
	@echo "✅ Konvertiert: $@"

# SCAD-Teile zu STL
$(STL_OUT)/%.stl: $(SCAD_PARTS)/%.scad
	@mkdir -p $(STL_OUT)
	@echo "⭢️  Erzeuge STL aus OpenSCAD: $<"
	openscad --hardwarnings -m render -D \$$fn=128 "$<" -o "$@"
	@echo "✅ Konvertiert: $@"

# SCAD-Teile zu 3MF
$(3MF_OUT)/%.3mf: $(SCAD_PARTS)/%.scad
	@mkdir -p $(3MF_OUT)
	@echo "⭢️  Erzeuge 3MF aus OpenSCAD: $<"
	openscad --hardwarnings -m render -D \$$fn=128 "$<" -o "$@"
	@echo "✅ Konvertiert: $@"

$(foreach i,$(FRAMES),\
  $(eval $(DOCS_DIR)/frame_$(i).png: $(SCAD_MODEL) $(PCB_STL_FILES))\
  $(eval $(DOCS_DIR)/frame_$(i).png:; \
    @mkdir -p $(DOCS_DIR); \
    echo "Rendering frame $(i) (step_full_manuell = $(i))"; \
    openscad -m render -D 'manuell=true' -D 'step_full_manuell=$(i)' -o $$@ $$<) \
)

# Aufräumen
clean:
	rm -rf $(STEP_OUT) $(STL_OUT) $(3MF_OUT)
	rm -rf $(FRAME_FILES)
	$(MAKE) -C $(FIRMWARE_DIR) clean

.PHONY: all clean flash monitor
