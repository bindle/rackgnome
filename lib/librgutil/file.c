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


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////
#ifdef __RACKGNOME_PMARK
#pragma mark - Prototypes
#endif

int rgu_fs_strexpand(struct rgu_file * fs, char **linep);


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
   size_t            pos;
   struct rgu_file * fs;

   assert(fsp != NULL);

   fs             = *fsp;
   *fsp           = (*fsp)->prev;
   if ((*fsp))
      (*fsp)->next   = NULL;

   if ((fs->argv))
   {
      for(pos = 0; ((fs->argv[pos])); pos++)
         free(fs->argv[pos]);
      free(fs->argv);
   };

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
      rgu_fs_perror(fs, "read(): %s", strerror(errno));
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
   if ((ptr = strchr(fs->buff, delimiter)) == NULL)
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
      rgu_perror(cnf, "%s: strdup(): %s", path);
      rgu_fs_close(&fs);
      return(-1);
   };


   if ((err = stat(path, &fs->sb)) == -1)
   {
      rgu_fs_perror(fs, "stat(): %s", strerror(errno));
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
         rgu_fs_perror(fs, "malloc(): %s", strerror(errno));
         rgu_fs_close(&fs);
         return(-1);
      };
   };


   // opens file for reading
   if ((fs->fd = open(fs->path, O_RDONLY)) == -1)
   {
      rgu_fs_perror(fs, "open(O_RDONLY): %s", strerror(errno));
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


ssize_t rgu_fs_splitline(struct rgu_file * fs, char * line, char *** argvp)
{
   int         err;
   size_t      size;
   size_t      pos;
   size_t      len;
   int         expand;
   char      * arg;

   assert(fs    != NULL);
   assert(line  != NULL);
   assert(argvp != NULL);

   // frees old results
   if ((fs->argv))
   {
      for(pos = 0; ((fs->argv[pos])); pos++)
         free(fs->argv[pos]);
      fs->argv[0] = NULL;
   };

   fs->argc = 0;
   len      = strlen(line);

   for(pos = 0; pos < len; pos++)
   {
      arg      = NULL;
      expand   = 0;

      switch(line[pos])
      {
         case '\0':
         case '#':
         return(fs->argc);

         case ' ':
         case '\t':
         case '\r':
         break;


         case '\'':
         pos++;
         arg   = &line[pos];
         while(line[++pos] !=  '\'')
         {
            if ( (pos >= len) || (line[pos] == '\0') )
            {
               rgu_fs_perror(fs, "unterminated quoted string");
               return(-1);
            };
         };
         line[pos] = '\0';
         break;


         case '"':
         expand = 1;
         pos++;
         arg   = &line[pos];
         while (line[++pos] !=  '"')
         {
            if ( (pos >= len) || (line[pos] == '\0') )
            {
               rgu_fs_perror(fs, "unterminated quoted string");
               return(-1);
            };
            if (line[pos] == '\\')
               pos++;
         };
         line[pos] = '\0';
         break;


         default:
         arg = &line[pos];
         while( ((line[pos])) && (pos < len) )
         {
            pos++;
            if (line[pos] == '#')
            {
               line[pos+1] = '#';
               line[pos]   = ' ';
            };
            if ((line[pos] == ' ') || (line[pos] == '\t'))
               line[pos] = '\0';
         };
         break;
      };

      if (!(arg))
         continue;

      size = sizeof(char *) * (size_t)(fs->argc+2);
      if ((fs->argv = realloc(*argvp, size)) == NULL)
      {
         rgu_fs_perror(fs, "realloc(): %s", strerror(errno));
         return(-1);
      };
      fs->argv[fs->argc+1] = NULL;
      *argvp               = fs->argv;

      if ((fs->argv[fs->argc] = strdup(arg)) == NULL)
      {
         rgu_fs_perror(fs, "strdup(): %s", strerror(errno));
         return(-1);
      };
      fs->argc++;

      if (!(expand))
         continue;

      if ((err = rgu_fs_strexpand(fs, &fs->argv[fs->argc-1])) == -1)
         return(-1);

   };

   return((ssize_t)fs->argc);
}


int rgu_fs_strexpand(struct rgu_file * fs, char **linep)
{
   ssize_t       size;
   size_t        pos;
   size_t        len;
   char        * line;
   char        * str;
   char          tmpstr[1024];

   assert(fs      != NULL);
   assert(linep   != NULL);

   line  = *linep;
   len   = strlen(line) + 1;

   for(pos = 0; pos < len; pos++)
   {
      if (line[pos] != '\\')
         continue;

      //    \\   expands to ‘\’
      //    \"   expands to ‘"’
      //    \n   expands to a newline
      //    \t   expands to a tab
      //    \d   expands to domain name
      //    \h   expands to hostname
      //    \f   expands to fully qualified hostname
      //    \p   expands to PID
      //    \P   expands to the sub-system name (dispatcher, poller, rackgnome)
      switch(line[pos+1])
      {
         case '\\':
         memmove(&line[pos], &line[pos+1], len-pos-1);
         break;

         case '"':
         memmove(&line[pos], &line[pos+1], len-pos-1);
         break;

         case 'n':
         memmove(&line[pos], &line[pos+1], len-pos-1);
         line[pos] = '\n';
         break;

         case 't':
         memmove(&line[pos], &line[pos+1], len-pos-1);
         line[pos] = '\t';
         break;

         case 'd':
         line[pos] = '\0';
         if ((size = asprintf(&str, "%s%s%s", line, fs->cnf->domainname, &line[pos+2])) == -1)
         {
            rgu_fs_perror(fs, "asprintf(domainname): %s", strerror(errno));
            return(-1);
         };
         snprintf(str, (size_t)(size+1), "%s%s%s", line, fs->cnf->domainname, &line[pos+2]);
         free(line);
         line     = str;
         *linep   = line;
         len  = (size_t)size + 1;
         pos += strlen(fs->cnf->domainname) - 1;
         break;

         case 'f':
         line[pos] = '\0';
         if ((size = asprintf(&str, "%s%s%s", line, fs->cnf->fqdn, &line[pos+2])) == -1)
         {
            rgu_fs_perror(fs, "asprintf(fqdn): %s", strerror(errno));
            return(-1);
         };
         snprintf(str, (size_t)(size+1), "%s%s%s", line, fs->cnf->fqdn, &line[pos+2]);
         free(line);
         line     = str;
         *linep   = line;
         len  = (size_t)size + 1;
         pos += strlen(fs->cnf->fqdn) - 1;
         break;

         case 'h':
         line[pos] = '\0';
         if ((size = asprintf(&str, "%s%s%s", line, fs->cnf->hostname, &line[pos+2])) == -1)
         {
            rgu_fs_perror(fs, "asprintf(hostname): %s", strerror(errno));
            return(-1);
         };
         snprintf(str, (size_t)(size+1), "%s%s%s", line, fs->cnf->hostname, &line[pos+2]);
         free(line);
         line     = str;
         *linep   = line;
         len  = (size_t)size + 1;
         pos += strlen(fs->cnf->hostname) - 1;
         break;

         case 'P':
         break;

         case 'p':
         snprintf(tmpstr, sizeof(tmpstr), "%i", getpid());
         line[pos] = '\0';
         if ((size = asprintf(&str, "%s%s%s", line, tmpstr, &line[pos+2])) == -1)
         {
            rgu_fs_perror(fs, "asprintf(pid): %s", strerror(errno));
            return(-1);
         };
         snprintf(str, (size_t)(size+1), "%s%s%s", line, tmpstr, &line[pos+2]);
         free(line);
         line     = str;
         *linep   = line;
         len  = (size_t)size + 1;
         pos += strlen(tmpstr) - 1;
         break;

         case'\0':
         rgu_fs_perror(fs, "missing keyword in escape sequence");
         return(-1);

         default:
         rgu_fs_perror(fs, "unknown keyword in escape sequence");
         return(-1);
      };
   };

   return(0);
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
