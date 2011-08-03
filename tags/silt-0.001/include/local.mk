D := include

$(D)/all:

$(D)/clean: CDIR := $(D)

$(D)/clean:
	rm -f $(CDIR)/*~

$(D)/install: CDIR := $(D)

$(D)/install:
	$(foreach h,$(wildcard $(CDIR)/*.h),cp $(h) $(INCLUDE_DIR);	\
	chmod 644 $(INCLUDE_DIR)/$(notdir $(h)); )
