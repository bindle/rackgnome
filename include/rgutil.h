/*
 *  Rack Gnome
 *  Copyright (C) 2016 David M. Syzdek <syzdek@bindlebinaries.com>.
 *
 *  @SYZDEK_BSD_LICENSE_START@
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are
 *  met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of David M. Syzdek nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 *  IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL DAVID M. SYZDEK BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 *  OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 *  SUCH DAMAGE.
 *
 *  @SYZDEK_BSD_LICENSE_END@
 */
#ifndef __RGUTIL_H
#define __RGUTIL_H 1
#undef __RACKGNOME_PMARK


///////////////
//           //
//  Headers  //
//           //
///////////////
#ifdef __RACKGNOME_PMARK
#pragma mark - Headers
#endif

#include <rackgnome.h>

#include <sys/time.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <stdarg.h>
#include <syslog.h>
#include <inttypes.h>


///////////////////
//               //
//  Definitions  //
//               //
///////////////////
#ifdef __RACKGNOME_PMARK
#pragma mark - Definitions
#endif

#define RGU_DANY        -1
#define RGU_DNONE       0x0000000000000000
#define RGU_DCONF       0x0000000000000001
#define RGU_DPROC       0x0000000000000002
#define RGU_DTHRD       0x0000000000000004
#define RGU_DCONN       0x0000000000000008
#define RGU_DPRBE       0x0000000000000010
#define RGU_DCACH       0x0000000000000020
#define RGU_DDB         0x0000000000000040
#define RGU_DGOLD       0x0000000000000080


//////////////////
//              //
//  Data Types  //
//              //
//////////////////
#ifdef __RACKGNOME_PMARK
#pragma mark - Data Types
#endif


typedef struct rgu_scheduler  rgu_scheduler;
typedef struct rgu_time_slice rgu_timeslice;
typedef struct rgu_job_slot   rgu_job;


typedef struct rgu_cnf rgu_cnf;
struct rgu_cnf
{
   uint64_t         debug;
   uint8_t          foreground;
   uint8_t          verbose;
   uint8_t          silent;
   uint8_t          openlog;
   uint32_t         pad32;
   char           * prog_name;
   char           * cnffile;
   char           * pidfile;
   char           * argsfile;
   char           * sockfile;
   char           * user_name;
   char           * group_name;
   char           * pwnam_buff;
   char           * grnam_buff;
   struct passwd    pwd;
   struct group     grp;
   size_t           pwnam_len;
   size_t           grnam_len;
};

//
//              Rack Gnome Scheduler/Time Slice Relationship
//                         (timer uses nanseconds)
//
//           +-------+     +-------+     +-------+     +-------+
//      /--> | slice | --> | slice | --> | slice | --> | slice | ---\
//      |    +-------+     +-------+     +-------+     +-------+    |
//      |                                                           |
//  +-------+      scheduler is a `struct rgutil_scheduler`     +-------+
//  | slice |      slice is a `struct rgutil_time_slice`        | slice |
//  +-------+      job is a `struct rgutil_job_slot`            +-------+
//      ^                                                           |
//      |    +-------+     +-------+     +-------+     +-------+    |
//      \--- | slice | <-- | slice | <-- | slice | <-- | slice | <--/
//           +-------+     +-------+     +-------+     +-------+
//                                                         ^
//  +-------+     +-----+-----+-----+                      |
//  | slice | --> | job | job | job |                +-----------+
//  +-------+     |-----|-----|-----|                | scheduler |
//                | job | job | job |                +-----------+
//                |-----|-----|-----|
//                | job | job | job |
//                +-----+-----+-----+
//


struct rgu_scheduler
{
   struct timespec     now;      // time when slice was last incremented
   size_t              slices;   // number of time slices in ring
   size_t              slots;    // max number of queued jobs per slice
   long                interval; // length of each time slice in nanoseconds (1e-9 seconds)
   rgu_timeslice       * ring;     // references current time slice in ring
};


struct rgu_time_slice
{
   size_t           job_len;     // number of job slots currently used
   rgu_timeslice    * next;        // references next time slice
   rgu_job      * job_slots;   // array of slots
};


struct rgu_job_slot
{
   uint64_t      job_type;
   uint64_t      job_id;
   void        * job_data;
};


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////
#ifdef __RACKGNOME_PMARK
#pragma mark -
#endif

// config functions
_RACKGNOME_F int  rgu_config_init(rgu_cnf ** cnfp, const char * prog_name);
_RACKGNOME_F void rgu_config_free(rgu_cnf * cnf);
_RACKGNOME_F int  rgu_config_parse(rgu_cnf * cnf);

// logging functions
_RACKGNOME_F void rgu_debug(rgu_cnf * cnf, uint64_t debug, const char * fmt, ...);
_RACKGNOME_F void rgu_log(rgu_cnf * cnf, int priority, const char * fmt, ...);
_RACKGNOME_F void rgu_vlog(rgu_cnf * cnf, int priority, const char * fmt, va_list args);
_RACKGNOME_F void rgu_vdebug(rgu_cnf * cnf, uint64_t debug, const char * fmt, va_list args);
_RACKGNOME_F void rgu_perror(rgu_cnf * cnf, const char * fmt, ...);
_RACKGNOME_F void rgu_perror_r(rgu_cnf * cnf, char * restrict str, size_t size, const char * fmt, ...);
_RACKGNOME_F void rgu_vperror(rgu_cnf * cnf, const char * fmt, va_list args);
_RACKGNOME_F void rgu_vperror_r(rgu_cnf * cnf, char * restrict str, size_t size, const char * fmt, va_list args);

// version functions
_RACKGNOME_F void rgu_version(rackgnome_ver const ** verp);
_RACKGNOME_F void rgu_version_assert(void);
_RACKGNOME_F void rgu_version_print(const char * prog_name);
_RACKGNOME_F void rgu_version_print_terse(const char * prog_name);

// scheduling functions
_RACKGNOME_F void rgu_clock_gettime(struct timespec * ts);



#endif /* rgutil_h */
