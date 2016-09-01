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
#define __LIBRGUTIL_LOGGER_C 1
#include "logger.h"


///////////////
//           //
//  Headers  //
//           //
///////////////
#ifdef __RACKGNOME_PMARK
#pragma mark - Headers
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdarg.h>
#include <unistd.h>
#include <syslog.h>
#include <stdio.h>
#include <errno.h>


/////////////////
//             //
//  Functions  //
//             //
/////////////////
#ifdef __RACKGNOME_PMARK
#pragma mark - Functions
#endif


void rgu_debug(rgu_cnf * cnf, uint64_t debug, const char * fmt, ...)
{
   va_list     args;

   assert(fmt != NULL);

   va_start(args, fmt);
   rgu_vdebug(cnf, debug, fmt, args);
   va_end(args);

   return;
}


void rgu_log(rgu_cnf * cnf, int priority, const char * fmt, ...)
{
   va_list       args;

   assert(fmt != NULL);

   va_start(args, fmt);
   rgu_vlog(cnf, priority, fmt, args);
   va_end(args);

   return;
}


void rgu_perror(rgu_cnf * cnf, const char * fmt, ...)
{
   va_list     args;
   char        str[1024];

   assert(fmt != NULL);

   va_start(args, fmt);
   rgu_vperror_r(cnf, str, sizeof(str), fmt, args);
   va_end(args);

   return;
}


void rgu_perror_r(rgu_cnf * cnf, char * restrict str, size_t size, const char * fmt, ...)
{
   va_list     args;

   assert(fmt != NULL);

   va_start(args, fmt);
   rgu_vperror_r(cnf, str, size, fmt, args);
   va_end(args);

   return;
}


void rgu_vdebug(rgu_cnf * cnf, uint64_t debug, const char * fmt, va_list args)
{
   uint64_t    cnf_debug;

   assert(fmt != NULL);

   cnf_debug = ((cnf)) ? cnf->debug : 0xffffffffffffffff;

   if ((cnf_debug & debug) == 0)
      return;

   rgu_vlog(cnf, LOG_DEBUG, fmt, args);

   return;
}


void rgu_vlog(rgu_cnf * cnf, int priority, const char * fmt, va_list args)
{
   uint8_t       olog;
   const char  * prog_name;

   assert(fmt != NULL);

   olog = ((cnf)) ? cnf->openlog : 0;
   if (olog != 0)
   {
      vsyslog(LOG_DEBUG, fmt, args);
      return;
   };

   prog_name = "rackgnome";
   if ((cnf))
      if ((cnf->prog_name))
         prog_name = cnf->prog_name;

   fprintf(stderr, "%s[%i]: ", prog_name, getpid());
   vfprintf(stderr, fmt, args);
   fprintf(stderr, "\n");

   return;
}


void rgu_vperror(rgu_cnf * cnf, const char * fmt, va_list args)
{
   char  str[1024];

   assert(fmt != NULL);

   rgu_vperror_r(cnf, str, sizeof(str), fmt, args);

   return;
}


void rgu_vperror_r(rgu_cnf * cnf, char * restrict str, size_t size, const char * fmt, va_list args)
{
   size_t     len;
   char     * errmsg;

   assert(str != NULL);
   assert(fmt != NULL);
   assert(size > 4);

   len      = size / 2;
   errmsg   = &str[len + 1];

   vsnprintf(str, len, fmt, args);
   strerror_r(errno, errmsg, len);

   rgu_log(cnf, LOG_ERR, "%s: %s", str, errmsg);

   return;
}

/* end of source */
