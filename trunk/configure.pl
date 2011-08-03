#!/usr/bin/env perl

##### enable perl's safety info #####
use strict;
use warnings;

##### global variables #####
my $cmdname = $0;
my $verbose = 0;
my $prefix = "/dev/null";
my $debugBuild = 0;
my $optLevel = 2;
my $archivename = "silt";

main();
exit(0);

sub main{
  parsecla();

  open FH, '> config.log' or die "Unable to open config.log";
  print FH $cmdname;
  foreach my $arg (@ARGV){ print FH ' ' . $arg; }
  print FH "\n";
  close FH;

  open FH, '> system.mk' or die "Unable to open system.mk";
  print FH "PREFIX := " . $prefix . "\n";
  print FH "ARCHIVE_NAME := " . $archivename . "\n";
  print FH "SYS_CPPFLAGS :=\n";
  print FH "SYS_CFLAGS :=";
  if($debugBuild){ print FH " -g"; }
  if($optLevel > 0){ print FH " -O" . $optLevel; }
  print FH "\n";
  print FH "SYS_LDFLAGS :=\n";
  close FH;

  `make deps > /dev/null 2>&1`;
}

sub parsecla{
  foreach my $arg (@ARGV){
    if($arg =~ /^-/){
      if($arg eq "-h" || $arg eq "--help"){ usage(); exit(0); }
      elsif($arg =~ /^--archivename=(.+)/){ $archivename = $1; }
      elsif($arg eq "--debug-build"){ $debugBuild = 1; }
      elsif($arg =~ /^--opt=(0|1|2|3)$/){ $optLevel = $1; }
      elsif($arg =~ /^--prefix=(.+)/){ $prefix = $1; }
      elsif($arg eq "--verbose"){ $verbose = 1; }
      else{ print "Illegal option: " . $arg . "\n"; exit(1); }
    }
    else{ print "Illegal positional argument: " . $arg . "\n"; exit(1); }
  }
}

sub usage{
  print "USAGE: " . $cmdname . " [OPTIONS]

DESCRIPTION: Configure the build environment

OPTIONS:
  -h|--help            print this message
  --archivename=<str>  define an archive name; the default is silt
  --debug-build        enable a debug build
  --opt=[0,1,2,3]      set the build optimization level; the default is 2
  --prefix=<dir>       set the installation directory
  --verbose            enable output verbosity
";
}
