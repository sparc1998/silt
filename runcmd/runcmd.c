#define _GNU_SOURCE // needed for sched.h and maybe something else
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <alloca.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <time.h>
#include <stdint.h>
#include <signal.h>
#include <ctype.h>
#include <sched.h>

#include "CStringHelp.h"
#include "TimeHelp.h"
#include "RunCmd.h"
#include "RunCmdInternal.h"
#include "ErrorHandling.h"

/**
 *  See usage() for a description and usage details of the runcmd
 *  program.  In the code below, we make use of 2 processes:
 *
 *  - parent process: the process started by main that oversees operation
 *  - app process: the process that execs the app to be run
 *
 *  General info:
 *
 *  - Child processes inherit signal handlers from the parent.  Of course
 *    they are destroyed when an exec is performed.
 *  - Unless specifically disabled, a signal of type s will be blocked if
 *    we are in a signal handler for a signal of type s.
 *  - Children inherit their parent's pgid.
 *  - If we receive signal s, enter s's signal handler, and then raise s
 *    in the signal handler, we'll enter the signal handler again, as soon
 *    as the signal handler exits the first time.
 *  - Forked children do not inherit alarms from their parent.
 *  - It is ok if we start a pipe for the child to write to the parent,
 *    and the parent waits before it reads.
 *  - Children inherit the parent's cpu affinity mask.
 */

////////////////////////////
///// Global variables /////
////////////////////////////
int setCpuAffinity = 0;
cpu_set_t affinityMask;
char* execDir = NULL;
char* stderrFile = NULL;
char* exitStatusFile = NULL;
char* stdinFile = NULL;
char* ldLibPath = NULL;
char* stdoutFile = NULL;
char* preload = NULL;
int recordRusage = 0;
unsigned sleepTime = 0;
int recordClockTime = 0;
unsigned timeoutVal = 0;
int verbose = 0;
char** cmdStart = NULL;
// If the parent pid is 0, no children have been forked.  If it is not 0,
// check the current pid versus the parent pid to determine if we are in
// the parent process.
pid_t parentPid = 0;
SignalHandlerState sigHandlerState = NOT_WAITING;
int appTimedOut = 0;
struct sigaction origSigAlrmAction;
struct sigaction origSigTermAction;

/////////////////////
///// Functions /////
/////////////////////

/**
 *  Print usage information.
 */
void usage(FILE* fp){
  fprintf(fp, "USAGE: runcmd [OPTIONS] -- <cmd> [<cmd args>]\n");
  fprintf(fp, "DESCRIPTION: Runs a command under various settings.\n");
  fprintf(fp, "OPTIONS:\n");
  fprintf(fp, "  %s,%s              print this help message\n",
	  HELP_CLA, HELP_SHORT_CLA);
  fprintf(fp, "  %s<cpu list>      restrict execution to the specified "
	  "command-separated cpu list, cpus are labeled 0 to (n-1)\n",
	  RESTRICT_CPUS_CLA "=");
  fprintf(fp, "  %s<dir>            set execution directory\n",
	  DIR_CLA "=");
  fprintf(fp, "  %s<file>           set file to store stderr\n",
	  ERR_CLA "=");
  fprintf(fp, "  %s<file>    set file to store exit status\n",
	  EXIT_STATUS_FILE_CLA "=");
  fprintf(fp, "  %s<file>            set file from which to read stdin\n",
	  IN_CLA "=");
  fprintf(fp, "  %s<val>        add value to LD_LIBRARY_PATH\n",
	  LDLIBPATH_CLA "=");
  fprintf(fp, "  %s<file>           set file to store stdout\n",
	  OUT_CLA "=");
  fprintf(fp, "  %s<val>        set LD_PRELOAD\n",
	  PRELOAD_CLA "=");
  fprintf(fp, "  %s               record resource usage with exit status\n",
	  RUSAGE_CLA);
  fprintf(fp, "  %s<secs>         sleep time before running command\n",
	  SLEEP_CLA "=");
  fprintf(fp, "  %s                 record clock time with exit status\n",
	  TIME_CLA);
  fprintf(fp, "  %s<secs>       timeout value for command\n",
	  TIMEOUT_CLA "=");
  fprintf(fp, "  %s              enable verbose output\n",
	  VERBOSE_CLA);
}

/**
 *  Parses command line arguments.
 */
void parseCla(int argc, char** argv){
  unsigned i;

  // Scan cla for location of app to run and to extract options
  // applying to this process.  Later we will restructure argv.
  for(i = 1; i < (unsigned)argc; ++i){
    char* arg = argv[i];
    char* ptrAfterPrefix;
    if(strcmp("--", arg) == 0){
      if((i+1) < argc){ cmdStart = &argv[i+1]; return; }
      else{ myerror("No command specified."); }
    }
    else if(arg[0] == '-'){
      if(strcmp(arg, HELP_CLA) == 0 || strcmp(arg, HELP_SHORT_CLA) == 0){
	usage(stdout);
	exit(RUNCMD_SUCCESS_EXIT_VAL);
      }
      else if((ptrAfterPrefix =
	       getPtr2StrAfterPrefix(arg, RESTRICT_CPUS_CLA "="))){
	setCpuAffinity = 1;
	CPU_ZERO(&affinityMask);
	populateCpuMask(ptrAfterPrefix);
      }
      else if((ptrAfterPrefix =
	       getPtr2StrAfterPrefix(arg, DIR_CLA "="))){
	execDir = ptrAfterPrefix;
      }
      else if((ptrAfterPrefix =
	       getPtr2StrAfterPrefix(arg, ERR_CLA "="))){
	stderrFile = ptrAfterPrefix;
      }
      else if((ptrAfterPrefix =
	      getPtr2StrAfterPrefix(arg, EXIT_STATUS_FILE_CLA "="))){
	exitStatusFile = ptrAfterPrefix;
      }
      else if((ptrAfterPrefix =
	       getPtr2StrAfterPrefix(arg, IN_CLA "="))){
	stdinFile = ptrAfterPrefix;
      }
      else if((ptrAfterPrefix =
	       getPtr2StrAfterPrefix(arg, LDLIBPATH_CLA "="))){
	ldLibPath = ptrAfterPrefix;
      }
      else if((ptrAfterPrefix =
	       getPtr2StrAfterPrefix(arg, OUT_CLA "="))){
	stdoutFile = ptrAfterPrefix;
      }
      else if((ptrAfterPrefix =
	       getPtr2StrAfterPrefix(arg, PRELOAD_CLA "="))){
	preload = ptrAfterPrefix;
      }
      else if(strcmp(arg, RUSAGE_CLA) == 0){
	recordRusage = 1;
      }
      else if((ptrAfterPrefix =
	       getPtr2StrAfterPrefix(arg, SLEEP_CLA "="))){
	sleepTime = (unsigned)strtoul(ptrAfterPrefix, NULL, 10);
      }
      else if(strcmp(arg, TIME_CLA) == 0){
	recordClockTime = 1;
      }
      else if((ptrAfterPrefix =
	       getPtr2StrAfterPrefix(arg, TIMEOUT_CLA "="))){
	timeoutVal = (unsigned)strtoul(ptrAfterPrefix, NULL, 10);
      }
      else if(strcmp(arg, VERBOSE_CLA) == 0){
	verbose = 1;
      }
      // Any other arguments are illegal
      else{
	myerror("Illegal argument: %s", arg);
      }
    }
    else{ myerror("Illegal argument: %s", arg); }
  }

  myerror("No command specified.");
}

/**
 *  Print the state of runcmd. This is used for debugging purposes.
 */
void printState(FILE* fp, char** argv){
  fprintf(fp, "runcmd state\n");
  fprintf(fp, "  record rusage:     %d\n", recordRusage);
  fprintf(fp, "  record clock time: %d\n", recordClockTime);
  if(preload){ fprintf(fp, "  preload:           %s\n", preload); }
  if(ldLibPath){ fprintf(fp, "  ld lib path:       %s\n", ldLibPath); }
  if(execDir){ fprintf(fp, "  exec dir:          %s\n", execDir); }
  if(stdinFile){ fprintf(fp, "  stdin file:        %s\n", stdinFile); }
  if(stdoutFile){ fprintf(fp, "  stdout file:       %s\n", stdoutFile); }
  if(stderrFile){ fprintf(fp, "  stderr file:       %s\n", stderrFile); }
  if(sleepTime > 0){ fprintf(fp, "  sleep time (s):    %u\n", sleepTime); }
  if(timeoutVal > 0){ fprintf(fp, "  timeout (s):       %u\n", timeoutVal); }
  if(exitStatusFile){fprintf(fp, "  exit status file:  %s\n", exitStatusFile);}
  if(setCpuAffinity){
    fprintf(fp, "  set cpu affinity: ");
    int i;
    for(i=0; i < CPU_SETSIZE; ++i){
      fprintf(fp, " %d", CPU_ISSET(i, &affinityMask));
    }
    fprintf(fp, "\n");
  }
  fprintf(fp, "  app & args:\n");
  unsigned i;
  for(i=0; cmdStart[i] != NULL; ++i){
    fprintf(fp, "                     %s\n", cmdStart[i]);
  }
}

/**
 *  This signal handler is designed to handle SIGALRM signals that we
 *  are using for sleeps and timeouts. It also forwards SIGTERM
 *  signals to children.
 */
void sigHandler(int s){
  if( parentPid == 0 || getpid() == parentPid){
    // Parent process

    if(s == SIGALRM && sigHandlerState == WAITING_FOR_SLEEP_SIGALRM){
      // We are waking up from a sleep
      sigHandlerState = NOT_WAITING;
    }
    else if(s == SIGALRM && sigHandlerState == WAITING_FOR_TIMEOUT_SIGALRM){
      // We just received the timeout notification
      appTimedOut = 1;
      sigHandlerState = IGNORE_SIGTERM;
      // Send SIGTERM to pgid
      if(kill(0, SIGTERM) != 0){
	myerror("Parent could not send SIGTERM to its process group.");
      }
    }
    else if(s == SIGTERM && sigHandlerState == IGNORE_SIGTERM){
      // We are timing out the application process.  To do so, we issued
      // a SIGTERM to the process group.  So, the parent needs to ignore
      // that SIGTERM
      sigHandlerState = NOT_WAITING;
    }
    else{
      // We did not send this signal from within runcmd.
      if(sigHandlerState <= TOO_SMALL_STATE ||
	 sigHandlerState >= TOO_LARGE_STATE){
	mywarn(0, "Illegal signal handler state.");
      }
      installOldSigHandlerAndReraise(s);
    }
  }
  else{
    // Application child process before exec

    installOldSigHandlerAndReraise(s);
  }
}

void installOldSigHandlerAndReraise(int s){
  struct sigaction* newAction = NULL;
  if(s == SIGALRM){ newAction = &origSigAlrmAction; }
  else if(s == SIGTERM){ newAction = &origSigTermAction; }
  else{
    // We shouldn't have registered any other signal handlers
    myerror("Signal handler received non-registered signal %s.", strsignal(s));
  }

  if(sigaction(s, newAction, NULL) != 0){
    myerror("Could not restore signal handler for %s.", strsignal(s));
  }
  if(raise(s) != 0){
    myerror("Could not raise signal %s.", strsignal(s));
  }
}

/**
 *  Using alarm and pause to create a sleep.
 */
void mysleep(unsigned secs){
  if(sigHandlerState != NOT_WAITING){
    myerror("Attempt to schedule a sleep when signal handler is not "
	    "in the not waiting state.");
  }
  sigHandlerState = WAITING_FOR_SLEEP_SIGALRM;
  alarm(secs);
  pause();
}

/**
 *  Convert a string to an unsigned 64-bit integer.
 */
uint64_t strtou64(char* s){
  uint64_t v = 0;
  while(isdigit(*s)){
    v = v*10 + (*s - '0');
    ++s;
  }
  return v;
}

/**
 *  Read through a comma separated-list of cpu ids and set
 *  affinityMask appropriately.
 */
void populateCpuMask(char* cpuList){
  char* current = cpuList;
  int stop = 0;
  for(; !stop; ++current){
    char* start = current;
    char c;
    for(c = *current; c != '\0' && c != ','; ++current, c = *current){}
    if(c == '\0'){ stop = 1; }
    else{ *current = '\0'; }
    int cpuNum = strtol(start, NULL, 10);
    myassert(cpuNum >= 0 && cpuNum < CPU_SETSIZE, 0, "Illegal cpu number.");
    CPU_SET(cpuNum, &affinityMask);
  }
}

/**
 *  Main runcmd logic.
 */
int main(int argc, char** argv){
  // Parse command line arguments.
  parseCla(argc, argv);
  if(verbose){ printState(stdout, cmdStart); }

  // Note, we try to do as much before the fork, for the app process,
  // as possible so it does not interfere with resource usage

  // Get pid and set gpid
  parentPid = getpid();
  if(setpgid(0, 0) != 0){ myerror("Unable to set group pid."); }

  // Set cpu affinity if asked to do so
  if(setCpuAffinity){
    myassert(sched_setaffinity(0, sizeof(cpu_set_t), &affinityMask) == 0,
	     0, "Unable to set cpu affinity.");
  }

  // Register signal handlers
  struct sigaction sa;
  sa.sa_handler = sigHandler;
  if(sigemptyset(&sa.sa_mask) != 0 || sigaddset(&sa.sa_mask, SIGALRM) != 0 ||
     sigaddset(&sa.sa_mask, SIGTERM) != 0){
    myerror("Could not properly set signal set.");
  }
  sa.sa_flags = 0;
  if(sigaction(SIGALRM, &sa, &origSigAlrmAction) != 0 ||
     sigaction(SIGTERM, &sa, &origSigTermAction) != 0){
    myerror("Could not register signal handler.");
  }

  // Sleep if asked to do so
  if(sleepTime > 0){ mysleep(sleepTime); }

  // Change directory
  if(execDir && chdir(execDir) != 0){
    myerror("Unable to change current directory to %s.", execDir);
  }

  // Open files for application's stdin, stdout & stderr, if necessary
  int appStdinFd = -1;
  if(stdinFile){
    appStdinFd = open(stdinFile, O_RDONLY);
    if(appStdinFd == -1){
      myerror("Could not open file %s.", stdinFile);
    }
  }
  int appStdoutFd = -1;
  if(stdoutFile){
    appStdoutFd = open(stdoutFile, O_WRONLY|O_CREAT|O_TRUNC,
		       S_IRUSR|S_IRGRP|S_IWUSR);
    if(appStdoutFd == -1){
      myerror("Could not open file %s.", stdoutFile);
    }
  }
  int appStderrFd = -1;
  if(stderrFile){
    appStderrFd = open(stderrFile, O_WRONLY|O_CREAT|O_TRUNC,
		       S_IRUSR|S_IRGRP|S_IWUSR);
    if(appStderrFd == -1){
      myerror("Could not open file %s.", stderrFile);
    }
  }
  
  // Compute values for environment variables
  char* newLdLibPath = NULL;
  if(ldLibPath){
    char* oldValue = getenv("LD_LIBRARY_PATH");
    if(oldValue == NULL || strlen(oldValue) == 0){
      newLdLibPath = (char*)alloca(strlen(ldLibPath) + 1);
      strcpy(newLdLibPath, ldLibPath);
    }
    else if(oldValue[0] == ':'){
      newLdLibPath = alloca(1 + strlen(ldLibPath) + strlen(oldValue) + 1);
      sprintf(newLdLibPath, ":%s%s", ldLibPath, oldValue);
    }
    else{
      newLdLibPath = alloca(strlen(ldLibPath) + 1 + strlen(oldValue) + 1);
      sprintf(newLdLibPath, "%s:%s", ldLibPath, oldValue);
    }
  }

  // If we are recording the clock time, open a pipe for the child
  // to communicate it to the parent
  int pipefd[2];
  if(recordClockTime){
    if(pipe(pipefd) != 0){ myerror("Could not open a pipe."); }
  }

  // Set an alarm for a timeout if asked to do so
  if(timeoutVal > 0){
    sigHandlerState = WAITING_FOR_TIMEOUT_SIGALRM;
    alarm(timeoutVal);
  }

  // Fork application process
  pid_t appForkRv = fork();
  if(appForkRv == 0){
    // Application child process

    // Setup stdin, stdout & stderr redirection
    if(stdinFile){
      myassert(close(STDIN_FILENO) == 0, 0, "Unable to close child's stdin.");
      myassert(dup2(appStdinFd, STDIN_FILENO) != -1, 0,
	       "Unable to dup2 stdin in child.");
    }
    if(stdoutFile){
      myassert(close(STDOUT_FILENO) == 0, 0, "Unable to close child's stdout.");
      myassert(dup2(appStdoutFd, STDOUT_FILENO) != -1, 0,
	       "Unable to dup2 stdout in child.");
    }
    if(stderrFile){
      myassert(close(STDERR_FILENO) == 0, 0, "Unable to close child's stderr.");
      myassert(dup2(appStderrFd, STDERR_FILENO) != -1, 0,
	       "Unable to dup2 stderr in child.");
    }

    // Set needed environment variables
    if(preload){
      myassert(setenv("LD_PRELOAD", preload, 1) == 0, 0,
	       "Unable to set LD_PRELOAD.");
    }
    if(ldLibPath){
      myassert(setenv("LD_LIBRARY_PATH", newLdLibPath, 1) == 0, 0,
	       "Unable to set LD_LIBRARY_PATH.");
    }

    // Record start time if we are recording clock time
    if(recordClockTime){
      FILE* pipeWriter = fdopen(pipefd[1], "w");
      myassert(pipeWriter, 0, "Could not open write end of pipe.");
      struct timespec startTime;
      myassert(clock_gettime(CLOCK_REALTIME, &startTime) == 0, 0,
	       "Could get start time.");
      myassert(fprintf(pipeWriter,
		       "%lu\n", structTimespec2nsecs(startTime)) >= 0, 0,
	       "Could not write start time to pipe.");
      if(fclose(pipeWriter) != 0){
	mywarn(0, "Unable to close write end of pipe.");
      }
    }

    // Exec application
    execvp(cmdStart[0], cmdStart);
    myerror("Exec failed.");
  }

  // Parent process
  
  // Check if fork failed
  myassert(appForkRv != -1, 0, "Fork failed.");

  // Close redirection files so parent does not inadvertently access them
  if(stdinFile && close(appStdinFd) != 0){
    mywarn(0, "Unable to close file %s.", stdinFile);
  }
  if(stdoutFile && close(appStdoutFd) != 0){
    mywarn(0, "Unable to close file %s.", stdoutFile);
  }
  if(stderrFile && close(appStderrFd) != 0){
    mywarn(0, "Unable to close file %s.", stderrFile);
  }

  // Wait on child
  int appProcStatus;
  struct rusage ru;
  if(recordRusage){
    if(wait4(appForkRv, &appProcStatus, 0, &ru) == -1 && !appTimedOut){
      myerror("Wait failed.");
    }
  }
  else{
    if(waitpid(appForkRv, &appProcStatus, 0) == -1 && !appTimedOut){
      myerror("Wait failed.");
    }
  }

  // Cancel alarm if we set one
  if(timeoutVal > 0){
    alarm(0);
    sigHandlerState = NOT_WAITING;
  }

  // Extract start time
  uint64_t startTime = 0;
  if(recordClockTime){
    FILE* pipeReader = fdopen(pipefd[0], "r");
    myassert(pipeReader, 0, "Unable to open read end of pipe.");
    // We add 2 to buf size, one for the '\0' and one for the '\n'
    char buf[UINT64_T_MAX_DIGITS + 2];
    myassert(fgets(buf, UINT64_T_MAX_DIGITS + 2, pipeReader), 0,
	     "Unable to read pipe.");
    startTime = strtou64(buf);
    if(fclose(pipeReader) != 0){
      mywarn(0, "Unable to close read end of pipe.");
    }
  }

  // Record finish time
  struct timespec endTime;
  if(recordClockTime){
    myassert(clock_gettime(CLOCK_REALTIME, &endTime) == 0, 0,
	     "Unable to record end time.");
  }

  // Record exit status
  if(exitStatusFile){
    FILE* exitStatusFp = fopen(exitStatusFile, "w");
    myassert(exitStatusFp, 0, "Unable to open file to write exit status.");

    if(appTimedOut){
      // Timeout termination
      fprintf(exitStatusFp, RUNCMD_EXIT_TYPE_PREFIX ": "
	      RUNCMD_TIMEOUT_EXIT "\n");
      fprintf(exitStatusFp, RUNCMD_RV_PREFIX ": -\n");
      fprintf(exitStatusFp, RUNCMD_SIGNAL_TERM_PREFIX ": -\n");
    }
    else if(WIFEXITED(appProcStatus)){
      // Normal termination
      fprintf(exitStatusFp, RUNCMD_EXIT_TYPE_PREFIX ": "
	      RUNCMD_NORMAL_EXIT "\n");
      fprintf(exitStatusFp, RUNCMD_RV_PREFIX ": %d\n",
	      WEXITSTATUS(appProcStatus));
      fprintf(exitStatusFp, RUNCMD_SIGNAL_TERM_PREFIX ": -\n");
    }
    else if(WIFSIGNALED(appProcStatus)){
      // Signal termination
      fprintf(exitStatusFp, RUNCMD_EXIT_TYPE_PREFIX ": "
	      RUNCMD_SIGNAL_EXIT "\n");
      fprintf(exitStatusFp, RUNCMD_RV_PREFIX ": -\n");
      fprintf(exitStatusFp, RUNCMD_SIGNAL_TERM_PREFIX ": %d %s\n",
	      WTERMSIG(appProcStatus), strsignal(WTERMSIG(appProcStatus)));
    }
    else{
      myerror("Unrecognized exit status %d.", appProcStatus);
    }
    
    if(recordClockTime){
      // clock time
      fprintf(exitStatusFp,
	      RUNCMD_CLOCK_TIME_PREFIX ": %lu" NSECS_STR "\n",
	      structTimespec2nsecs(endTime) - startTime);
    }
    
    if(recordRusage){
      // Record resource usage
      
      // user time
      fprintf(exitStatusFp,
	      RUNCMD_USER_TIME_PREFIX ": %lu" USECS_STR "\n",
	      structTimeval2usecs(ru.ru_utime));
      // system time
      fprintf(exitStatusFp,
	      RUNCMD_SYSTEM_TIME_PREFIX ": %lu" USECS_STR "\n",
	      structTimeval2usecs(ru.ru_stime));
    }
    
    if(fclose(exitStatusFp) != 0){
      mywarn(0, "Unable to close file %s.", exitStatusFile);
    }
  }
  
  return RUNCMD_SUCCESS_EXIT_VAL;
}
