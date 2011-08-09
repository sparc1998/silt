# Copyright (c) 2011, Ryan M. Lefever
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above
#       copyright notice, this list of conditions and the following
#       disclaimer in the documentation and/or other materials provided
#       with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# Include system makefile generated by configure
include system.mk

# Paths to tools
CC := gcc
CXX := g++
FIX_DEP_FILES := utils/fix_dfile.pl
GREP_RECURSIVE := utils/grep-recursive

# Directory paths
SUBDIRS :=				\
		getctypeinfo		\
		include			\
		include/c++		\
		include_internal	\
		include_internal/c++	\
		runcmd			\
		utils
BIN_DIR := $(PREFIX)/bin
INCLUDE_DIR := $(PREFIX)/include

# Flags
CPPFLAGS := $(SYS_CPPFLAGS) -Iinclude -Iinclude_internal
CFLAGS := $(SYS_CFLAGS) -Wall
CXXFLAGS := $(SYS_CXXFLAGS) -Wall
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
CPP2O =		$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $(1) $(2)
C2EXE =		$(CC) -o $(1) $(2) $(LDFLAGS)
CPP2EXE =	$(CXX) -o $(1) $(2) $(LDFLAGS)
INSTALL_EXE =	cp $(1) $(BIN_DIR);					\
		chmod 755 $(BIN_DIR)/$(notdir $(1))
INSTALL_H =	cp $(1) $(INCLUDE_DIR);					\
		chmod 644 $(INCLUDE_DIR)/$(notdir $(1))

# Set all as the default target
all:

# Include local makefiles
include $(foreach d,$(SUBDIRS),$(d)/local.mk)

all: $(foreach d,$(SUBDIRS),$(d)/all)

clean: $(foreach d,$(SUBDIRS),$(d)/clean)
	rm -f *~

install: all installdirs $(foreach d,$(SUBDIRS),$(d)/install)

deps:

realclean: clean $(foreach d,$(SUBDIRS),$(d)/realclean)

archive:
	rm -rf temp/$(ARCHIVE_NAME) temp/$(ARCHIVE_NAME).tgz
	mkdir temp/$(ARCHIVE_NAME)
	cp -a `find . -maxdepth 1 -mindepth 1 | grep -v ./temp`		\
	temp/$(ARCHIVE_NAME)
	rm -rf `find temp/$(ARCHIVE_NAME) -name .svn`
	cd temp/$(ARCHIVE_NAME); ./configure.pl; make realclean
	rm -f temp/$(ARCHIVE_NAME)/system.mk
	cd temp; tar cfz $(ARCHIVE_NAME).tgz $(ARCHIVE_NAME)

findattn:
	$(GREP_RECURSIVE) --ignore-dir=.svn,./temp --ignore-ft=o,d ATTN

installdirs:
	mkdir -p $(BIN_DIR)
	chmod 755 $(BIN_DIR)
	mkdir -p $(INCLUDE_DIR)
	chmod 755 $(INCLUDE_DIR)

%.d: %.c
	$(call C2D,$@,$<)

%.d: %.cpp
	$(call CPP2D,$@,$<)

%.o: %.c
	$(call C2O,$@,$<)

%.o: %.cpp
	$(call CPP2O,$@,$<)
