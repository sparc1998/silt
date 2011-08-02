D := include_internal

$(D)/all:

$(D)/clean: CDIR := $(D)

$(D)/clean:
	rm -f $(CDIR)/*~

$(D)/install:

