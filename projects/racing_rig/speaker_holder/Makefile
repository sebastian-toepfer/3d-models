STL_OUT := 3d/stl
3MF_OUT := 3d/3mf
SCAD_PARTS := 3d/openscad/parts

# Alle .scad-Dateien in parts finden
SCAD_PART_FILES := $(wildcard $(SCAD_PARTS)/*.scad)
SCAD_PART_FILES := $(filter-out %.template.scad, $(SCAD_PART_FILES))

define SCAD_TO_STL
$(STL_OUT)/$(basename $(notdir $(1))).stl
endef

define SCAD_TO_3MF
$(3MF_OUT)/$(basename $(notdir $(1))).3mf
endef

# Regeln erzeugen
PRINTABLE_STL_FILES := $(foreach scad,$(SCAD_PART_FILES),$(call SCAD_TO_STL,$(scad)))
PRINTABLE_3MF_FILES := $(foreach scad,$(SCAD_PART_FILES),$(call SCAD_TO_3MF,$(scad)))

# Standard-Ziel
all: $(PRINTABLE_STL_FILES) $(PRINTABLE_3MF_FILES)

3mf: $(PRINTABLE_3MF_FILES)
stl: $(PRINTABLE_STL_FILES)

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

# Aufräumen
clean:
	rm -rf $(STL_OUT) $(3MF_OUT)

.PHONY: all clean
