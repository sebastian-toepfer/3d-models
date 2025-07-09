# Alle direkten Unterverzeichnisse, die ein Makefile enthalten
subdirs := $(shell find . -maxdepth 1 -type d ! -name . -exec test -f "{}/Makefile" \; -print)

all: $(subdirs)

$(subdirs):
	$(MAKE) -C $@

clean:
	@for dir in $(subdirs); do \
		$(MAKE) -C $$dir clean || true; \
	done

.PHONY: all clean $(subdirs)
