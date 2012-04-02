# Copyright (c) 2012, Ryan M. Lefever
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

# DESCRIPTION: This package exports several common functions to be used
#              by SiLT Perl programs.
package SiltCommon;

use 5;

# enable perl's safety info
use strict;
use warnings;

use Term::ANSIColor;

require Exporter;

our @ISA = qw(Exporter);

our @EXPORT_OK = ();

our @EXPORT = qw(setverbose verbose
		 setnorun norun
		 printcmd runcmd runcmd_backtics);

our $VERSION = "0.01";

# Run commands
BEGIN {
  my $verbose = 0;
  my $norun = 0;

  sub setverbose{ $verbose = shift @_; }
  sub verbose{ return $verbose; }
  sub setnorun{ $norun = shift @_; }
  sub norun{ return $norun; }

  sub printcmd{
    my $cmd = shift @_;
    print color "blue bold";
    print "CMD: " . $cmd . "\n";
    print color "reset";
  }

  sub runcmd{
    my $cmd = shift @_;
    if($verbose){ printcmd($cmd); }
    if(!$norun){ system($cmd); }
  }

  sub runcmd_backtics{
    my $cmd = shift @_;
    if($verbose){ printcmd($cmd); }
    if(!$norun){ return `$cmd`; }
    return "";
  }
}

1;
__END__
