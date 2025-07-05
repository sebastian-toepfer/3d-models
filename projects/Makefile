projects := $(shell find . -maxdepth 1 -type d ! -name . -exec test -f "{}/Makefile" \; -print)

all: $(projects)

$(projects):
	$(MAKE) -C $@

clean:
	@for dir in $(projects); do \
		$(MAKE) -C $$dir clean || true; \
	done

.PHONY: all clean $(projects)
