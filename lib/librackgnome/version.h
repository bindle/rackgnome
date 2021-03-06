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
#ifndef __LIBRACKGNOME_VERSION_H
#define __LIBRACKGNOME_VERSION_H 1


///////////////
//           //
//  Headers  //
//           //
///////////////
#ifdef __RACKGNOME_PMARK
#pragma mark - Headers
#endif

#include "librackgnome.h"


///////////////////
//               //
//  Definitions  //
//               //
///////////////////
#ifdef __RACKGNOME_PMARK
#pragma mark - Definitions
#endif

#ifndef GIT_PACKAGE_MAJOR
#   define GIT_PACKAGE_MAJOR 0
#endif
#ifndef GIT_PACKAGE_MINOR
#   define GIT_PACKAGE_MINOR 0
#endif
#ifndef GIT_PACKAGE_PATCH
#   define GIT_PACKAGE_PATCH 0
#endif
#ifndef GIT_PACKAGE_BUILD
#   define GIT_PACKAGE_BUILD ""
#endif
#ifndef GIT_PACKAGE_BUILD
#   define GIT_PACKAGE_VERSION_NUMBER 0.000000
#endif
#ifndef GIT_PACKAGE_VERSION
#   define GIT_PACKAGE_VERSION "0.0.0"
#endif
#ifndef GIT_PACKAGE_BUILD
#   define GIT_PACKAGE_BUILD "gzzzzzz"
#endif
#ifndef GIT_PACKAGE_VERSION_BUILD
#   define GIT_PACKAGE_VERSION_BUILD (GIT_PACKAGE_VERSION "." GIT_PACKAGE_BUILD)
#endif


#ifndef LIB_VERSION_CURRENT
#define LIB_VERSION_CURRENT  0
#endif
#ifndef LIB_VERSION_REVISION
#define LIB_VERSION_REVISION 0xffffffff
#endif
#ifndef LIB_VERSION_AGE
#define LIB_VERSION_AGE      0xffffffff
#endif
#ifndef LIB_VERSION_INFO
#define LIB_VERSION_INFO "4294967295:0:0"
#endif
#ifndef LIB_RELEASE_INFO
#define LIB_RELEASE_INFO "4294967295:0:0"
#endif



#endif /* rgtimer_h */
