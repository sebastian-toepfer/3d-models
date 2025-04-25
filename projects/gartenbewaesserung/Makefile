KICAD_DIR := kicad
STEP_OUT := 3d/step

# Alle .kicad_pcb-Dateien finden
PCB_FILES := $(shell find $(KICAD_DIR) -type f -name '*.kicad_pcb')

# Hilfsfunktion: nur Dateinamen extrahieren und Zielpfad erzeugen
define PCB_TO_STEP
$(STEP_OUT)/board_$(notdir $(basename $(1))).step
endef

# Erzeuge Liste mit expliziten Regeln: STEP_ZIEL: PCB_QUELLE
STEP_RULES := $(foreach pcb,$(PCB_FILES),$(call PCB_TO_STEP,$(pcb)):$(pcb))

# Liste der STEP-Ziele
STEP_FILES := $(foreach rule,$(STEP_RULES),$(firstword $(subst :, ,$(rule))))

# Standard-Ziel
all: $(STEP_FILES)

# Generiere jede Regel explizit
$(foreach rule,$(STEP_RULES),\
  $(eval $(firstword $(subst :, ,$(rule))): $(lastword $(subst :, ,$(rule))) ) \
)

# Kommandos zum Generieren
$(STEP_OUT)/board_%.step:
	@mkdir -p $(STEP_OUT)
	@echo "⭢️  Erzeuge STEP für: $<"
	kicad-cli pcb export step "$<" --output "$@" --include-tracks --include-zones
	@echo "✅ Exportiert: $@"

# Aufräumen
clean:
	rm -rf $(STEP_OUT)

.PHONY: all clean
