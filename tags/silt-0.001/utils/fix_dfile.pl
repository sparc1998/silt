#!/usr/bin/env perl

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

# enable perl's safety info
use strict;
use warnings;

# Adds directory qualifiers and bytecode file names to the dependency
# targets in .d files.

my $inFile = $ARGV[0];
my $outFile = $ARGV[1];
my $cSrcFile = $ARGV[2];
my $bareOFile = $cSrcFile;
$bareOFile =~ s/\.(c|cpp)$/\.o/;
if ($bareOFile =~ /.+\/([^\/]+)/){ $bareOFile = $1; }
my $tmp = $outFile;
$tmp =~ s/\.d/\.o/;
my $dependerFiles = $tmp;
$tmp = $outFile;
$tmp =~ s/\.d/\.dbg\.o/;
$dependerFiles = $dependerFiles . " " . $tmp;
$tmp = $outFile;
$tmp =~ s/\.d/\.opt\.o/;
$dependerFiles = $dependerFiles . " " . $tmp;
$tmp = $outFile;
$tmp =~ s/\.d/\.std\.o/;
$dependerFiles = $dependerFiles . " " . $tmp;
$tmp = $outFile;
$tmp =~ s/\.d/\.dbgopt\.o/;
$dependerFiles = $dependerFiles . " " . $tmp;
$tmp = $outFile;
$tmp =~ s/\.d/\.dbgpic\.o/;
$dependerFiles = $dependerFiles . " " . $tmp;
$tmp = $outFile;
$tmp =~ s/\.d/\.optpic\.o/;
$dependerFiles = $dependerFiles . " " . $tmp;
$tmp = $outFile;
$tmp =~ s/\.d/\.stdpic\.o/;
$dependerFiles = $dependerFiles . " " . $tmp;
$tmp = $outFile;
$tmp =~ s/\.d/\.dbgoptpic\.o/;
$dependerFiles = $dependerFiles . " " . $tmp;
$tmp = $outFile;
$tmp =~ s/\.d/\.dbg\.bc/;
$dependerFiles = $dependerFiles . " " . $tmp;
$tmp = $outFile;
$tmp =~ s/\.d/\.opt\.bc/;
$dependerFiles = $dependerFiles . " " . $tmp;
$tmp = $outFile;
$tmp =~ s/\.d/\.std\.bc/;
$dependerFiles = $dependerFiles . " " . $tmp;
$tmp = $outFile;
$tmp =~ s/\.d/\.dbgopt\.bc/;
$dependerFiles = $dependerFiles . " " . $tmp;
$tmp = $outFile;
$tmp =~ s/\.d/\.api\.bc/;
$dependerFiles = $dependerFiles . " " . $tmp;
open(IFH, $inFile);
open(OFH, "> " . $outFile);
my $line;
while($line = <IFH>){
  $line =~ s/$bareOFile *:/$dependerFiles:/;
  print OFH $line;
}
close OFH;
close IFH;
