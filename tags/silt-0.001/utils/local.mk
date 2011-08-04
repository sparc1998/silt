D := utils

$(D)_EXES :=	$(D)/grep-recursive					\
		$(D)/path						\
		$(D)/replace

$(D)/all:

$(D)/clean: CDIR := $(D)

$(D)/clean:
	rm -f $(CDIR)/*~

$(D)/install: CDIR := $(D)

$(D)/install:
	$(foreach exe,$($(CDIR)_EXES),$(call INSTALL_EXE,$(exe));)
