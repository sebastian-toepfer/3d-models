all: projects 

projects:
	$(MAKE) -C projects

clean:
	$(MAKE) -C projects clean || true; \

.PHONY: all clean projects
