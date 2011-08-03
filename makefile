# Include system makefile generated by configure.pl
include system.mk

# Tools
CC := gcc
FIX_DEP_FILES := utils/fix_dfile.pl

# Directories
SUBDIRS :=	include			\
		include_internal	\
		runcmd			\
		utils
BIN_DIR := $(PREFIX)/bin
INCLUDE_DIR := $(PREFIX)/include

# Flags
CPPFLAGS := $(SYS_CPPFLAGS) -Iinclude -Iinclude_internal
CFLAGS := $(SYS_CFLAGS) -Wall
LDFLAGS := $(SYS_LDFLAGS)

# Commands
C2D =		@set -e; rm -f $(1);					\
		$(CC) -MM $(CPPFLAGS)					\
		$(2) > $(1).$$$$;					\
		$(FIX_DEP_FILES) $(1).$$$$ $(1) $(2);			\
		rm -f $(1).$$$$
CPP2D =		@set -e; rm -f $(1);					\
		$(CC) -MM $(CPPFLAGS)					\
		$(2) > $(1).$$$$;					\
		$(FIX_DEP_FILES) $(1).$$$$ $(1) $(2);			\
		rm -f $(1).$$$$
C2O =		$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $(1) $(2)
GEN_EXE =	$(CC) -o $(1) $(2) $(LDFLAGS)

# Make all the default target
all:

# Include local makefiles
include $(foreach d,$(SUBDIRS),$(d)/local.mk)

all: $(foreach d,$(SUBDIRS),$(d)/all)

clean: $(foreach d,$(SUBDIRS),$(d)/clean)
	rm -f *~

install: installdirs $(foreach d,$(SUBDIRS),$(d)/install)

archive:
	rm -rf temp/$(ARCHIVE_NAME) temp/$(ARCHIVE_NAME).tgz
	mkdir temp/$(ARCHIVE_NAME)
	cp -a `find . -maxdepth 1 -mindepth 1 | grep -v ./temp`		\
	temp/$(ARCHIVE_NAME)
	rm -rf `find temp/$(ARCHIVE_NAME) -name .svn`
	cd temp/$(ARCHIVE_NAME); ./configure.pl; make clean
	rm -f temp/$(ARCHIVE_NAME)/system.mk
	rm -f `find temp/$(ARCHIVE_NAME) -name "*.d"`
	cd temp; tar cfz $(ARCHIVE_NAME).tgz $(ARCHIVE_NAME)

installdirs:
	mkdir -p $(INCLUDE_DIR)
	chmod 755 $(INCLUDE_DIR)

%.d: %.c
	$(call C2D,$@,$<)

%.d: %.cpp
	$(call CPP2D,$@,$<)

%.o: %.c
	$(call C2O,$@,$<)
