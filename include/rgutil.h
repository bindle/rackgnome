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


//////////////////
//              //
//  Data Types  //
//              //
//////////////////
#ifdef __RACKGNOME_PMARK
#pragma mark - Data Types
#endif


typedef struct rgutil_pkg_info rgutil_pkginfo;
struct rgutil_pkg_info
{
   const uint64_t   api_current;
   const uint64_t   api_revision;
   const uint64_t   api_age;

   const char     * api_version_info;
   const char     * api_release_info;

   const uint64_t   pkg_major;
   const uint64_t   pkg_minor;
   const uint64_t   pkg_patch;
   const double     pkg_version_number;

   const char     * pkg_version;
   const char     * pkg_version_build;
   const char     * pkg_build;
};


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////
#ifdef __RACKGNOME_PMARK
#pragma mark -
#endif

_RACKGNOME_F void rgutil_version(rackgnome_ver const ** verp);
_RACKGNOME_F void rgutil_version_print(const char * prog_name);
_RACKGNOME_F void rgutil_version_print_terse(const char * prog_name);
_RACKGNOME_F void rgutil_clock_gettime(struct timespec * ts);



#endif /* rgutil_h */
