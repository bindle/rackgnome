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
#ifndef __RACKGNOME_H
#define __RACKGNOME_H 1
#undef __RACKGNOME_PMARK


///////////////
//           //
//  Headers  //
//           //
///////////////
#ifdef __RACKGNOME_PMARK
#pragma mark - Headers
#endif

#include <inttypes.h>


///////////////////
//               //
//  Definitions  //
//               //
///////////////////
#ifdef __RACKGNOME_PMARK
#pragma mark - Definitions
#endif


//////////////
//          //
//  Macros  //
//          //
//////////////
#ifdef __RACKGNOME_PMARK
#pragma mark - Macros
#endif

#undef RACKGNOME_C_DECLS
#if defined(__cplusplus) || defined(c_plusplus)
#   define _RACKGNOME_I             extern "C" inline
#   define RACKGNOME_C_DECLS        "C"          ///< exports as C functions
#   define RACKGNOME_BEGIN_C_DECLS  extern "C" {    ///< exports as C functions
#   define RACKGNOME_END_C_DECLS    }               ///< exports as C functions
#else
#   define _RACKGNOME_I             inline
#   define RACKGNOME_C_DECLS        /* empty */  ///< exports as C functions
#   define RACKGNOME_BEGIN_C_DECLS  /* empty */     ///< exports as C functions
#   define RACKGNOME_END_C_DECLS    /* empty */     ///< exports as C functions
#endif


// Exports function type
//#define _RACKGNOME_I inline RACKGNOME_C_DECLS
#ifdef WIN32
#   ifdef _LIB_LIBRACKGNOME_H
#      define _RACKGNOME_F   extern RACKGNOME_C_DECLS __declspec(dllexport)   ///< used for library calls
#      define _RACKGNOME_V   extern RACKGNOME_C_DECLS __declspec(dllexport)   ///< used for library calls
#   else
#      define _RACKGNOME_F   extern RACKGNOME_C_DECLS __declspec(dllimport)   ///< used for library calls
#      define _RACKGNOME_V   extern RACKGNOME_C_DECLS __declspec(dllimport)   ///< used for library calls
#   endif
#else
#   ifdef _LIB_LIBRACKGNOME_H
#      define _RACKGNOME_F   /* empty */                                      ///< used for library calls
#      define _RACKGNOME_V   extern RACKGNOME_C_DECLS                         ///< used for library calls
#   else
#      define _RACKGNOME_F   extern RACKGNOME_C_DECLS                         ///< used for library calls
#      define _RACKGNOME_V   extern RACKGNOME_C_DECLS                         ///< used for library calls
#   endif
#endif


//////////////////
//              //
//  Data Types  //
//              //
//////////////////
#ifdef __RACKGNOME_PMARK
#pragma mark - Data Types
#endif


typedef struct rackgnome_version rackgnomever;
struct rackgnome_version
{
   const uint64_t   api_current;
   const uint64_t   api_revision;
   const uint64_t   api_age;

   const uint64_t   pkg_major;
   const uint64_t   pkg_minor;
   const uint64_t   pkg_patch;
   const double     pkg_version_number;

   const char     * api_version_info;
   const char     * api_release_info;

   const char     * pkg_version;
   const char     * pkg_version_build;
   const char     * pkg_build;
};


/////////////////
//             //
//  Variables  //
//             //
/////////////////
#ifdef __RACKGNOME_PMARK
#pragma mark - Variables
#endif


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////
#ifdef __RACKGNOME_PMARK
#pragma mark - Prototypes
#endif

_RACKGNOME_F void rackgnome_version(rackgnomever const ** ver);


#endif /* end of header */
