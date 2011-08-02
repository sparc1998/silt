D := runcmd

$(D)/all: $(D)/runcmd

$(D)/clean: CDIR := $(D)

$(D)/clean:
	rm -f $(CDIR)/*~ $(CDIR)/*.o $(CDIR)/runcmd

$(D)/install: CDIR := $(D)

$(D)/install:
	cp $(CDIR)/runcmd $(BIN_DIR)
	chmod 755 $(BIN_DIR)/runcmd

include $(subst .c,.d,$(wildcard $(D)/*.c))

$(D)/runcmd: LDFLAGS := $(LDFLAGS) -lrt

$(D)/runcmd: $(D)/runcmd.o
	$(call GEN_EXE,$@,$^)
