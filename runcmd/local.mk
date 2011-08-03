D := runcmd

$(D)_EXES := $(D)/runcmd

$(D)/all: $($(D)_EXES)

$(D)/clean: CDIR := $(D)

$(D)/clean:
	rm -f $(CDIR)/*~ $(CDIR)/*.o $($(CDIR)_EXES)

$(D)/install: CDIR := $(D)

$(D)/install:
	$(foreach exe,$($(CDIR)_EXES),$(call INSTALL_EXE,$(exe));)

include $(subst .c,.d,$(wildcard $(D)/*.c))

$(D)/runcmd: LDFLAGS := $(LDFLAGS) -lrt

$(D)/runcmd: $(D)/runcmd.o
	$(call GEN_EXE,$@,$^)
