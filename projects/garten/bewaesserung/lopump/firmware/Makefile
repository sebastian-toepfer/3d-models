# Makefile für firmware/ (PlatformIO)
all:
	@$(MAKE) codestyle
	platformio run

upload:
	platformio run --target upload

monitor:
	platformio device monitor

clean:
	rm -rf .pio build

SRC_FILES := $(shell find src lib -type f \( -name '*.c' -o -name '*.cpp' \))
CLANG_FORMAT := clang-format
FORMAT_STYLE ?= file

.PHONY: codestyle
codestyle:
ifeq ($(CI),true)
	@echo "🔍 Checking code format (CI mode)..."
	@$(CLANG_FORMAT) --dry-run --Werror -style=$(FORMAT_STYLE) $(SRC_FILES)
else
	@echo "🧹 Formatting source files (local mode)..."
	@$(CLANG_FORMAT) -i -style=$(FORMAT_STYLE) $(SRC_FILES)
endif

.PHONY: all upload monitor clean

