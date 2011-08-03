D := utils

$(D)/all:

$(D)/clean: CDIR := $(D)

$(D)/clean:
	rm -f $(CDIR)/*~

$(D)/install:

