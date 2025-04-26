KICAD_DIR := kicad
STL_OUT := 3d/stl

# Alle .kicad_pcb-Dateien finden
PCB_FILES := $(shell find $(KICAD_DIR) -type f -name '*.kicad_pcb')

# Hilfsfunktion: nur Dateinamen extrahieren und Zielpfad erzeugen
define PCB_TO_STL
$(STL_OUT)/board_$(notdir $(basename $(1))).stl
endef

# Erzeuge Liste mit expliziten Regeln: STL_ZIEL: PCB_QUELLE
STL_RULES := $(foreach pcb,$(PCB_FILES),$(call PCB_TO_STL,$(pcb)):$(pcb))

# Liste der STL-Ziele
STL_FILES := $(foreach rule,$(STL_RULES),$(firstword $(subst :, ,$(rule))))

# Standard-Ziel
all: $(STL_FILES)

# Generiere jede Regel explizit
$(foreach rule,$(STL_RULES), \
  $(eval $(firstword $(subst :, ,$(rule))): $(lastword $(subst :, ,$(rule)))) \
)

# Kommandos zum Generieren
$(STL_OUT)/board_%.stl:
	@mkdir -p $(STL_OUT)
	@echo "⭢️  Erzeuge STL für: $<"
	kicad-cli pcb export stl "$<" --output "$@" --grid-origin --include-tracks --include-zones
	@echo "✅ Exportiert: $@"

# Aufräumen
clean:
	rm -rf $(STEP_OUT)

.PHONY: all clean
