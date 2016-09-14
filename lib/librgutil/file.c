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
#define __LIBRGUTIL_FILE_C 1
#include "file.h"


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
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>


/////////////////
//             //
//  Functions  //
//             //
/////////////////
#ifdef __RACKGNOME_PMARK
#pragma mark - Functions
#endif

int rgu_fs_close(struct rgu_file ** fsp)
{
   struct rgu_file * fs;

   assert(fsp != NULL);

   fs             = *fsp;
   *fsp           = (*fsp)->prev;
   if ((*fsp))
      (*fsp)->next   = NULL;

   if ((fs->path))   free(fs->path);
   if ((fs->buff))   free(fs->buff);
   if (fs->fd != -1) close(fs->fd);

   free(fs);

   return(0);
}


int rgu_fs_closestack(struct rgu_file ** fsp)
{
   int                 err;
   struct rgu_file   * fs;

   fs = *fsp;

   while(fs != NULL)
   {
      if ((err = rgu_fs_close(&fs)) != 0)
      {
         *fsp = fs;
         return(-1);
      };
   };

   *fsp = fs;

   return(0);
}


ssize_t rgu_fs_read(struct rgu_file * fs)
{
   ssize_t    len;

   assert(fs         != NULL);

   // rotate processed data to begin
   memmove(fs->buff, &fs->buff[fs->buff_proc], fs->buff_read - fs->buff_proc);
   fs->buff_read -= fs->buff_proc;
   fs->buff_proc  = 0;

   // fill buffer from file
   if ((len = read(fs->fd, &fs->buff[fs->buff_read], (fs->buff_size - fs->buff_read - 1))) < 0)
   {
      rgu_fs_perror(fs, "read()");
      return(-1);
   };
   fs->buff_read += (size_t)len;

   return(len);
}


ssize_t rgu_fs_readdelim(struct rgu_file * fs, char ** linep, int delimiter)
{
   ssize_t    len;
   char     * ptr;

   assert(linep      != NULL);
   assert(fs         != NULL);
   assert(delimiter  != 0);

   if ((len = rgu_fs_read(fs)) == -1)
      return(-1);

   // look for next delimitor
   if ((ptr = index(fs->buff, delimiter)) == NULL)
   {
      // verify we are not out of buffer space
      if (fs->buff_read >= fs->buff_size)
      {
         rgu_fs_perror(fs, "buffer too small to read line");
         return(-1);
      };

      // assume we are at end of file and update meta data
      fs->buff[fs->buff_read] = '\0';
      fs->buff_proc           = fs->buff_read;
   } else {
      fs->buff_proc  = (size_t)(ptr - fs->buff + 1);
      ptr[0]         = '\0';
   };

   *linep         = fs->buff;
   fs->line++;

   return((ssize_t)fs->buff_proc);
}


ssize_t rgu_fs_readline(struct rgu_file * fs, char ** linep)
{
   ssize_t  len;

   assert(linep      != NULL);
   assert(fs         != NULL);

   if ((len = rgu_fs_readdelim(fs, linep, '\n')) == -1)
      return(-1);

   while(len > 0)
   {
      switch((*linep)[0])
      {
         case '\0':
         case '#':
         case '\n':
         len = rgu_fs_readdelim(fs, linep, '\n');
         break;

         case ' ':
         case '\t':
         case '\r':
         len--;
         (*linep)++;
         break;

         default:
         return(len);
      };
      if (len == 0)
         len = rgu_fs_readdelim(fs, linep, '\n');
   };

   return(len);
}


int rgu_fs_open(rgu_cnf * cnf, const char * path, uint64_t flags,
   struct rgu_file ** fsp)
{
   int                 err;
   struct rgu_file   * fs;
   struct rgu_file   * ptr;


   assert(cnf  != NULL);
   assert(path != NULL);


   if ((fs = malloc(sizeof(struct rgu_file))) == NULL)
   {
      rgu_perror(cnf, "malloc(): %s", strerror(errno));
      return(-1);
   };
   bzero(fs, sizeof(struct rgu_file));
   fs->cnf     = cnf;
   fs->prev    = *fsp;
   fs->top     = (!(fs->prev)) ? fs : fs->prev->top;
   fs->fd      = -1;
   fs->flags   = flags;


   // copies file name
   if ((fs->path = strdup(path)) == NULL)
   {
      rgu_perror(cnf, "%s: strdup()", path);
      rgu_fs_close(&fs);
      return(-1);
   };


   if ((err = stat(path, &fs->sb)) == -1)
   {
      rgu_fs_perror(fs, "stat()");
      rgu_fs_close(&fs);
      return(-1);
   };


   // checks for include loops
   for(ptr = fs; ptr->prev != NULL; ptr = ptr->prev)
   {
      if ( (fs->sb.st_dev == ptr->prev->sb.st_dev) &&
           (fs->sb.st_ino == ptr->prev->sb.st_ino) )
      {
         rgu_fs_perror(fs, "open(): already on file stack");
         rgu_fs_close(&fs);
         return(-1);
      };
   };


   // creates initial read buffer
   if ((fs->flags & RGU_FS_NOBUFF) == 0)
   {
      fs->buff_size = (size_t)fs->sb.st_blksize * 4;
      if (fs->sb.st_size < (fs->sb.st_blksize * 4))
         fs->buff_size = (size_t)fs->sb.st_size + 1;
      if ((fs->buff = malloc(fs->buff_size)) == NULL)
      {
         rgu_fs_perror(fs, "malloc()");
         rgu_fs_close(&fs);
         return(-1);
      };
   };


   // opens file for reading
   if ((fs->fd = open(fs->path, O_RDONLY)) == -1)
   {
      rgu_fs_perror(fs, "open(O_RDONLY)");
      rgu_fs_close(&fs);
      return(-1);
   };


   if ((*fsp))
      (*fsp)->next = fs;
   *fsp = fs;


   return(0);
}


void rgu_fs_perror(struct rgu_file * fs, const char * fmt, ...)
{
   va_list             args;
   char                str[4096];

   assert(fs  != NULL);
   assert(fmt != NULL);

   va_start(args, fmt);
   rgu_fs_perror_r(fs, str, sizeof(str), fmt, args);
   va_end(args);

   return;
}


void rgu_fs_perror_r(struct rgu_file * fs, char * str,
   size_t size, const char * fmt, ...)
{
   va_list             args;

   assert(fs  != NULL);
   assert(fmt != NULL);
   assert(str != NULL);
   assert(size > 4);

   va_start(args, fmt);
   rgu_fs_vperror_r(fs, str, size, fmt, args);
   va_end(args);

   return;
}


void rgu_fs_vperror_r(struct rgu_file * fs, char * str, size_t size,
   const char * fmt, va_list args)
{
   struct rgu_file   * ptr;
   char              * msg;
   size_t              len;

   assert(fs  != NULL);
   assert(fmt != NULL);
   assert(str != NULL);
   assert(size > 4);

   len = size / 4;
   msg = &str[len+1];

   for (ptr = fs->top; ptr != NULL; ptr = ptr->next)
   {
      if (ptr->line != 0)
         rgu_log(fs->cnf, LOG_ERR, "In file included from %s:%zu", ptr->path, ptr->line);
      else
         rgu_log(fs->cnf, LOG_ERR, "In file included from %s", ptr->path);
   };

   vsnprintf(msg, len, fmt, args);

   if (fs->line == 0)
      rgu_perror_r(fs->cnf, str, len, "%s: %s", fs->path, msg);
   else
      rgu_perror_r(fs->cnf, str, len, "%s: %zu: %s", fs->path, fs->line, msg);

   return;
}


/* end of source */
