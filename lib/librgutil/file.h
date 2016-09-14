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
#ifndef __LIBRGUTIL_FILE_H
#define __LIBRGUTIL_FILE_H 1


///////////////
//           //
//  Headers  //
//           //
///////////////
#ifdef __RACKGNOME_PMARK
#pragma mark - Headers
#endif

#include "librgutil.h"

#include <sys/stat.h>


///////////////////
//               //
//  Definitions  //
//               //
///////////////////
#ifdef __RACKGNOME_PMARK
#pragma mark - Definitions
#endif

#define RGU_FS_NOBUFF   0x0001


//////////////////
//              //
//  Data Types  //
//              //
//////////////////
#ifdef __RACKGNOME_PMARK
#pragma mark - Data Types
#endif

struct rgu_file
{
   int                 fd;
   int                 padint;
   uint64_t            flags;
   size_t              line;        ///< current line number
   size_t              buff_size;   ///< size of allocated buffer
   size_t              buff_proc;   ///< length of processed data
   size_t              buff_read;   ///< length of read data
   struct stat         sb;
   char              * path;
   char              * buff;
   struct rgu_file   * prev;
   struct rgu_file   * next;
   struct rgu_file   * top;
   rgu_cnf           * cnf;
};


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////
#ifdef __RACKGNOME_PMARK
#pragma mark -
#endif

int  rgu_fs_close(struct rgu_file ** fsp);
int  rgu_fs_closestack(struct rgu_file ** fsp);
ssize_t rgu_fs_read(struct rgu_file * fs);
ssize_t rgu_fs_readdelim(struct rgu_file * fs, char ** linep,
   int delimiter);
ssize_t rgu_fs_readline(struct rgu_file * fs, char ** linep);
int  rgu_fs_open(rgu_cnf * cnf, const char * path, uint64_t flags,
      struct rgu_file ** fsp);
void rgu_fs_perror(struct rgu_file * fs, const char * fmt, ...);
void rgu_fs_perror_r(struct rgu_file * fs, char * str, size_t size,
      const char * fmt, ...);
void rgu_fs_vperror_r(struct rgu_file * fs, char * str, size_t size,
      const char * fmt, va_list args);

#endif /* file.h */
