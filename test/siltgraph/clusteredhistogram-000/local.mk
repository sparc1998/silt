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

D := test/siltgraph/clusteredhistogram-000

$(D)_TARGET := $(D)/genhistogram
$(D)_RUNTEST_DIR := temp/clusteredhistogram-000

$(D)/all: $($(D)_TARGET)

$(D)/clean: CDIR := $(D)

$(D)/clean:
	rm -f $(CDIR)/*~ $(CDIR)/*.o $($(CDIR)_TARGET)

$(D)/install: $($(D)_TARGET).bininstall

$(D)/realclean: CDIR := $(D)

$(D)/realclean: $(D)/clean
	rm -f $(CDIR)/*.d $(CDIR)/*.d.*

$(D)/test: CDIR := $(D)

$(D)/test: $(D)/all
	@echo '===== Test start for $(CDIR) ====='
	rm -rf $($(CDIR)_RUNTEST_DIR)
	mkdir $($(CDIR)_RUNTEST_DIR)
	sh -c '$($(CDIR)_TARGET) $($(CDIR)_RUNTEST_DIR); echo Return value: $$?'
	@echo '===== Test end for $(CDIR)   ====='

include $(subst .cpp,.d,$(wildcard $(D)/*.cpp))

$($(D)_TARGET): $(D)/genhistogram.o siltgraph/libsiltgraph.a
	$(call CPP2EXE,$@,$^)
