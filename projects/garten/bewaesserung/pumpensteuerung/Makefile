KICAD_DIR := kicad
SCAD_MODEL := 3d/openscad/models/gehaeuse.scad
DOCS_DIR := docs/assembly
FRAMES := 1 2 3 4 5
FRAME_FILES := $(addprefix $(DOCS_DIR)/frame_,$(addsuffix .png,$(FRAMES)))
KICAD_SCH = kicad/pumpensteuerung.kicad_sch
NETLIST = netlist.xml
PINMAP_H = firmware/include/pinmap.h
BOARD_C  = firmware/src/platform/board/board.c
CODEGEN_DEPS := $(PINMAP_H) $(BOARD_C)

include ../../../../shared/mk/kicad.mk
include ../../../../shared/mk/3d.mk
include ../../../../shared/mk/firmware.mk

ALL_TARGETS += $(FRAME_FILES)
CLEAN_DIRS  += $(NETLIST) $(PINMAP_H) $(BOARD_C) $(FRAME_FILES)

$(NETLIST): $(KICAD_SCH)
	kicad-cli sch export netlist -o $(NETLIST) $(KICAD_SCH)

$(PINMAP_H) $(BOARD_C): $(NETLIST) tools/pinmap/generate_board.py
	python3 tools/pinmap/generate_board.py --platform mkrwan1310 --mcu-ref MCU1 --netlist $(NETLIST) --out-h $(PINMAP_H) --out-c $(BOARD_C)

frames: $(FRAME_FILES)

$(foreach i,$(FRAMES),\
  $(eval $(DOCS_DIR)/frame_$(i).png: $(SCAD_MODEL) $(PCB_STL_FILES))\
  $(eval $(DOCS_DIR)/frame_$(i).png:; \
    @mkdir -p $(DOCS_DIR); \
    echo "Rendering frame $(i) (step_full_manuell = $(i))"; \
    openscad -m render -D 'manuell=true' -D 'step_full_manuell=$(i)' -o $$@ $$<) \
)

include ../../../../shared/mk/common.mk
