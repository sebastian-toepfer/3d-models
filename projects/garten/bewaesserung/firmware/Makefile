# Makefile für firmware/ (PlatformIO)
all:
	@$(MAKE) codestyle
	@$(MAKE) test
	@$(MAKE) build

test:
	platformio test

build:
	platformio run -e mkrwan1310

upload:
	platformio run -e mkrwan1310 --target upload

monitor:
	platformio device -e mkrwan1310 monitor

clean:
	rm -rf .pio build

SRC_FILES := $(shell find src lib test -type f \( -name '*.c' -o -name '*.cpp' -o -name '*.h' \))
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

.PHONY: all test build upload monitor clean

