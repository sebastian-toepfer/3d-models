# Common rules for projects

# Collect targets from modules
ALL_TARGETS ?=
CLEAN_DIRS ?=
CLEAN_EXTRAS ?=

all: $(ALL_TARGETS)

clean: $(CLEAN_EXTRAS)
	@rm -rf $(CLEAN_DIRS)

.PHONY: all clean $(CLEAN_EXTRAS)
.DEFAULT_GOAL := all

