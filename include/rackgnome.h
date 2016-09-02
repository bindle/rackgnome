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
 *   @file rackgnome.h
 *   Rack Gnome's public API
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


/// @ingroup rg_version
/// @struct rackgnome_version
/// @brief Struct for storing API information and package information for
/// packages, libaries, and protocols.
///
/// API information is in the format of _current:revision:age_. Where:
///
///    * _current_   is the current version of the API
///                  (see [api_current](@ref rackgnome_version::api_current)).
///
///    * _revision_  is the revision of the source code implmenting the API
///                  (see [api_revision](@ref rackgnome_version::api_revision)).
///
///    * _age_       is the number of previous API versions which are
///                  compatible with the current API version
///                  (see [api_age](@ref rackgnome_version::api_age)).
///
/// Package information is in the format of _x.y.z.gbbbbbb_
/// (see [pkg_version](@ref rackgnome_version::pkg_version)). Where:
///
///    * _x_        is the major version number of the current package release.
///                 (see [pkg_major](@ref rackgnome_version::pkg_major)).
///
///    * _y_        is the minor version number of the current package release.
///                 (see [pkg_minor](@ref rackgnome_version::pkg_minor)).
///
///    * _z_        is the patch level of the current package release.
///                 (see [pkg_patch](@ref rackgnome_version::pkg_patch)).
///
///    * _gbbbbbb_  is the version control system's commit identifier of the
///                 current package release.
///                 (see [pkg_build](@ref rackgnome_version::pkg_build)).
///
/// @see rackgnome_version
///
struct rackgnome_version
{
   const uint64_t   api_current;    ///< The current version of the API.
   const uint64_t   api_revision;   ///< Revision of source code implementing the current version of the API
   const uint64_t   api_age;        ///< The number of previous API versions compatible with the current version of the API.

   const char     * api_string;     ///< String representation of the API information.

   const uint64_t   pkg_major;      ///< The major version number of the current package release.
   const uint64_t   pkg_minor;      ///< The minor version number of the current package release.
   const uint64_t   pkg_patch;      ///< The patch level of the current package release.
   const char     * pkg_build;      ///< The version control system's commit identifier of the current package release.

   const char     * pkg_string;     ///< String representation of the package version.
};
typedef struct rackgnome_version rackgnome_ver;


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
#pragma mark -
#endif

/**
 *  @}
 *  @defgroup rg_version Public Version Functions
 *  @brief Functions which report API and program versions.
 *  @{
 */
#ifdef __RACKGNOME_PMARK
#pragma mark Version Prototypes
#endif


/// Returns the API/version information for the Rack Gnome library.
/// @param[out] verp       Reference to pointer used to track configuration.
/// @see struct rackgnome_version
_RACKGNOME_F void rackgnome_version(rackgnome_ver const ** verp);


/** @} */
#endif /* end of header */
