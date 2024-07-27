OPENSCAD=/usr/bin/openscad

BUILDDIR  = generated
SOURCEDIR = designs

STRUCTURE            := $(shell find $(SOURCEDIR) -type d)
SRCFILES             := $(addsuffix /*, $(STRUCTURE))
SRCFILES             := $(wildcard $(SRCFILES))
JSONFILES            := $(filter %json, $(SRCFILES))
PARAMETERFILES       := $(JSONFILES:%.json=%)

OPENSCADFILES        := $(filter %scad, $(SRCFILES))
PARAMETERINIZEDFILES := $(filter $(PARAMETERFILES), $(OPENSCADFILES))
OPENSCADFILES        := $(filter-out $(PARAMETERINIZEDFILES), $(OPENSCADFILES))

TARGETS              := $(subst $(SOURCEDIR), $(BUILDDIR), $(OPENSCADFILES:%.scad=%.scad.stl))

.PHONY: all clean pull

all: ${TARGETS}

$(BUILDDIR)/%.stl: $(addprefix $(SOURCEDIR)/,%)
	$(OPENSCAD) --hardwarnings -m make -D \$fn=128 -o $@ $<
	cp $@ $(BUILDDIR)/new_stls/

pull:
	git pull -r

clean:
	rm -f ${TARGETS}

