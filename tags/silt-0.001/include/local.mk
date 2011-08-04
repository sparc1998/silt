D := include

$(D)/all:

$(D)/clean: CDIR := $(D)

$(D)/clean:
	rm -f $(CDIR)/*~

$(D)/install: CDIR := $(D)

$(D)/install:
	$(foreach h,$(wildcard $(CDIR)/*.h),$(call INSTALL_H,$(h));)
