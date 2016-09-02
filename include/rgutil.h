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
/**
 *   @file rgutil.h
 *   Rack Gnome's internal API
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

/// @ingroup rgu_log
/// @{
#define RGU_DANY        -1                   ///< Enable all debug messages
#define RGU_DNONE       0x0000000000000000   ///< Do not enable any debug messages (default)
#define RGU_DCONF       0x0000000000000001   ///< enable configuration parsing messages
#define RGU_DPROC       0x0000000000000002   ///< enable process management messages
#define RGU_DTHRD       0x0000000000000004   ///< enable thread management messages
#define RGU_DCONN       0x0000000000000008   ///< enable connection/socket management messages
#define RGU_DPRBE       0x0000000000000010   ///< enable probe debug messages
#define RGU_DCACH       0x0000000000000020   ///< enable cache debug messages
#define RGU_DDB         0x0000000000000040   ///< enable database debug messages
#define RGU_DGOLD       0x0000000000000080   ///< enable Goldogrin debug messages
/// @}


//////////////////
//              //
//  Data Types  //
//              //
//////////////////
#ifdef __RACKGNOME_PMARK
#pragma mark - Data Types
#endif


/// @ingroup rgu_conf
/// @brief Provides configuration information for internal utilities and
/// applications.
struct rgu_cnf
{
   uint64_t         debug;       ///< debug level of proram
   uint8_t          foreground;  ///< if a daemon, determines whether to run in forground or background
   uint8_t          verbose;     ///< controls verbosity of output if running in foreground
   uint8_t          silent;      ///< disables messages if running in foreground
   uint8_t          openlog;     ///< indicates wether logs have been initialized
   uint32_t         pad32;
   char           * prog_name;   ///< name of running program
   char           * cnffile;     ///< path to configuration file, if not in default location
   char           * pidfile;     ///< path to use when creating PID files
   char           * argsfile;    ///< path to use when greating argument files (i.e. .args)
   char           * sockfile;    ///< path to use when open UNIX domain socket
   char           * user_name;   ///< username to use when invoking setuid().
   char           * group_name;  ///< group name to use when invoking setgid().
   char           * pwnam_buff;
   char           * grnam_buff;
   struct passwd    pwd;         ///< passwd data for `user_name`
   struct group     grp;         ///< group data for `group_name`
   size_t           pwnam_len;
   size_t           grnam_len;
};
typedef struct rgu_cnf rgu_cnf;


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

struct rgu_job
{
   uint64_t      job_type;
   uint64_t      job_id;
   void        * job_data;
};


struct rgu_scheduler
{
   struct timespec        now;      ///< time when slice was last incremented
   size_t                 slices;   ///< number of time slices in ring
   size_t                 slots;    ///< max number of queued jobs per slice
   long                   interval; ///< length of each time slice in nanoseconds (1e-9 seconds)
   struct rgu_timeslice * ring;     ///< references current time slice in ring
};


struct rgu_timeslice
{
   size_t                 job_len;     ///< number of job slots currently used
   struct rgu_timeslice * next;        ///< references next time slice
   struct rgu_job       * job_slots;   ///< array of slots
};


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////
#ifdef __RACKGNOME_PMARK
#pragma mark -
#endif

/**
 *  @defgroup rgu_conf Internal Configuration Functions
 *  @brief Functions which parse configuration files and manage configuration
 *         resources.
 *  @{
 */
#ifdef __RACKGNOME_PMARK
#pragma mark Configuration Prototypes
#endif

/// Allocates and initializes configuration struct.
/// @param[out] cnfp       Reference to pointer used to track configuration.
/// @param[in]  prog_name  Name of program to use in logs and messages.
/// @return If successful, `rgu_config_init()` returns 0. It returns -1 if an
///         error was encountered and will record details to the log.
/// @see rgu_config_free, rgu_config_parse
_RACKGNOME_F int rgu_config_init(rgu_cnf ** cnfp, const char * prog_name);


/// Frees memory and resources used by the configuration struct referenced by `cnf`.
/// @param[in] cnf         Pointer to the librgutil state data.
/// @see rgu_config_init
_RACKGNOME_F void rgu_config_free(rgu_cnf * cnf);


/// Parses configuration files and applies default values to undefined options.
/// @param[in] cnf         Pointer to the librgutil state data.
/// @return If successful, `rgu_config_init()` returns 0. It returns -1 if an
///         error was encountered and will record details to the log.
/// @see rgu_config_init, rgu_config_free
_RACKGNOME_F int rgu_config_parse(rgu_cnf * cnf);


/**
 *  @}
 *  @defgroup rgu_log Internal Logging Functions
 *  @brief Functions which process log messages.
 */
#ifdef __RACKGNOME_PMARK
#pragma mark Logging Prototypes
#endif

_RACKGNOME_F void rgu_debug(rgu_cnf * cnf, uint64_t debug, const char * fmt, ...);
_RACKGNOME_F void rgu_log(rgu_cnf * cnf, int priority, const char * fmt, ...);
_RACKGNOME_F void rgu_vlog(rgu_cnf * cnf, int priority, const char * fmt, va_list args);
_RACKGNOME_F void rgu_vdebug(rgu_cnf * cnf, uint64_t debug, const char * fmt, va_list args);
_RACKGNOME_F void rgu_perror(rgu_cnf * cnf, const char * fmt, ...);
_RACKGNOME_F void rgu_perror_r(rgu_cnf * cnf, char * restrict str, size_t size, const char * fmt, ...);
_RACKGNOME_F void rgu_vperror(rgu_cnf * cnf, const char * fmt, va_list args);
_RACKGNOME_F void rgu_vperror_r(rgu_cnf * cnf, char * restrict str, size_t size, const char * fmt, va_list args);


/**
 *  @defgroup rgu_version Internal Version Functions
 *  @brief Functions which report API and program versions.
 */
#ifdef __RACKGNOME_PMARK
#pragma mark Version Prototypes
#endif

_RACKGNOME_F void rgu_version(rackgnome_ver const ** verp);
_RACKGNOME_F void rgu_version_assert(void);
_RACKGNOME_F void rgu_version_print(const char * prog_name);
_RACKGNOME_F void rgu_version_print_terse(const char * prog_name);


/**
 *  @defgroup rgu_schedule Internal Scheduling Functions
 *  @brief Functions which schedule jobs and manage job queues
 */
#ifdef __RACKGNOME_PMARK
#pragma mark Scheduling Prototypes
#endif

// scheduling functions
_RACKGNOME_F void rgu_clock_gettime(struct timespec * ts);



#endif /* rgutil_h */
