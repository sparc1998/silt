// Copyright (c) 2011, Ryan M. Lefever
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// 
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials provided
//       with the distribution.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef _RUNCMDINTERNAL_H_
#define _RUNCMDINTERNAL_H_

/**
 *  @file
 *
 *  This file defines information for use with the runcmd program.
 */

#include <stdint.h>
#include <signal.h>
#include <sched.h>

// Contants
#define HELP_SHORT_CLA           "-h"
#define HELP_CLA                 "--help"
#define VERBOSE_CLA              "--verbose"
#define PRELOAD_CLA              "--preload"
#define LDLIBPATH_CLA            "--libpath"
#define DIR_CLA                  "--dir"
#define IN_CLA                   "--in"
#define OUT_CLA                  "--out"
#define ERR_CLA                  "--err"
#define SLEEP_CLA                "--sleep"
#define TIMEOUT_CLA              "--timeout"
#define EXIT_STATUS_FILE_CLA     "--exitstatus"
#define RUSAGE_CLA               "--rusage"
#define TIME_CLA                 "--time"
#define RESTRICT_CPUS_CLA        "--cpus"

typedef enum {
  TOO_SMALL_STATE = 0,
  NOT_WAITING,
  WAITING_FOR_SLEEP_SIGALRM,
  WAITING_FOR_TIMEOUT_SIGALRM,
  IGNORE_SIGTERM,
  TOO_LARGE_STATE
} SignalHandlerState;

// Global variables
extern int debug;
extern int recordRusage;
extern int recordClockTime;
extern char* preload;
extern char* ldLibPath;
extern char* execDir;
extern char* stdinFile;
extern char* stdoutFile;
extern char* stderrFile;
extern unsigned sleepTime;
extern unsigned timeoutVal;
extern char* exitStatusFile;
extern int setCpuAffinity;
extern cpu_set_t affinityMask;
extern pid_t parentPid;
extern SignalHandlerState sigHandlerState;
extern int appTimedOut;
extern struct sigaction origSigAlrmAction;
extern struct sigaction origSigTermAction;

// Functions
void usage(FILE* fp);
void parseCla(int argc, char** argv);
void printState(FILE* fp, char** argv);
void sigHandler(int s);
void installOldSigHandlerAndReraise(int s);
void mysleep(unsigned secs);
uint64_t strtou64(char* s);
void populateCpuMask(char* cpuList);
int main(int argc, char** argv);

#endif // _RUNCMDINTERNAL_H_
