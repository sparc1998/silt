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

#ifndef _RUNCMD_H_
#define _RUNCMD_H_

// DESCRIPTION: This file defines values for interacting with the RUNCMD
//              binary.

#define RUNCMD_EXIT_TYPE_PREFIX         "exit_type"
#define RUNCMD_NORMAL_EXIT              "normal"
#define RUNCMD_SIGNAL_EXIT              "signal"
#define RUNCMD_TIMEOUT_EXIT             "timeout"
#define RUNCMD_RV_PREFIX                "rv"
#define RUNCMD_SIGNAL_TERM_PREFIX       "signal_termination"
#define RUNCMD_CLOCK_TIME_PREFIX        "clock_time"
#define RUNCMD_USER_TIME_PREFIX         "user_time"
#define RUNCMD_SYSTEM_TIME_PREFIX       "system_time"

#define RUNCMD_SUCCESS_EXIT_VAL         0

#endif // _RUNCMD_H_
