#!/usr/bin/env perl

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
