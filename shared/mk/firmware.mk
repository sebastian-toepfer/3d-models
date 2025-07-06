# Helper rules for PlatformIO firmware
FIRMWARE_DIR ?= firmware
CODEGEN_DEPS ?=

ALL_TARGETS += firmware
CLEAN_EXTRAS += firmware_clean

codegen: $(CODEGEN_DEPS)
	@:

firmware: codegen
	$(MAKE) -C $(FIRMWARE_DIR)

flash: firmware
	$(MAKE) -C $(FIRMWARE_DIR) upload

monitor:
	$(MAKE) -C $(FIRMWARE_DIR) monitor

firmware_clean:
	$(MAKE) -C $(FIRMWARE_DIR) clean

.PHONY: firmware flash monitor firmware_clean codegen
