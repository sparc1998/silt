#ifndef _RUNCMDINTERNAL_H_
#define _RUNCMDINTERNAL_H_

#include <stdint.h>

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
