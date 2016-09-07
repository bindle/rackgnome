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
#define __LIBRGUTIL_CONF_C 1
#include "conf.h"


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
#include <syslog.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "file.h"


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////
#ifdef __RACKGNOME_PMARK
#pragma mark -
#endif

void rgutil_config_free_str(char ** pptr);
int  rgutil_config_hostname(rgu_cnf * cnf);


/////////////////
//             //
//  Functions  //
//             //
/////////////////
#ifdef __RACKGNOME_PMARK
#pragma mark - Functions
#endif

void rgu_config_free(rgu_cnf * cnf)
{
   assert(cnf != NULL);

   rgutil_config_free_str(&cnf->prog_name);
   rgutil_config_free_str(&cnf->hostname);
   rgutil_config_free_str(&cnf->domainname);
   rgutil_config_free_str(&cnf->fqdn);
   rgutil_config_free_str(&cnf->cnffile);
   rgutil_config_free_str(&cnf->pidfile);
   rgutil_config_free_str(&cnf->argsfile);
   rgutil_config_free_str(&cnf->sockfile);
   rgutil_config_free_str(&cnf->user_name);
   rgutil_config_free_str(&cnf->group_name);
   rgutil_config_free_str(&cnf->pwnam_buff);
   rgutil_config_free_str(&cnf->grnam_buff);

   bzero(cnf, sizeof(rgu_cnf));

   free(cnf);

   return;
}


void rgutil_config_free_str(char ** pptr)
{
   assert(pptr != NULL);
   if ((*pptr))
   {
      free(*pptr);
      *pptr = NULL;
   };
   return;
}


int rgutil_config_hostname(rgu_cnf * cnf)
{
   int                 err;
   int                 gai;
   long                hostlen;
   char              * host;
   char              * idx;
   const char        * cidx;
   struct addrinfo     hints;
   struct addrinfo   * info;

   assert(cnf != NULL);

   if ((hostlen = sysconf(_SC_HOST_NAME_MAX)) == -1)
   {
      rgu_perror(cnf, "sysconf(_SC_HOST_NAME_MAX)");
      return(-1);
   };

   if ((host = malloc((size_t)hostlen)) == NULL)
   {
      rgu_perror(cnf, "malloc()");
      return(-1);
   };

   if ((err = gethostname(host, (size_t)hostlen)) == -1)
   {
      rgu_perror(cnf, "gethostname()");
      free(host);
      return(-1);
   };

   bzero(&hints, sizeof(hints));
   hints.ai_flags    = AI_CANONNAME|AI_ALL|AI_V4MAPPED;
   hints.ai_family   = AF_INET6;
   hints.ai_socktype = SOCK_STREAM;

   if ((gai = getaddrinfo(host, NULL, &hints, &info)) != 0)
   {
      rgu_log(cnf, LOG_ERR, "getaddrinfo(): %s", gai_strerror(gai));
      free(host);
      return(-1);
   };
   free(host);

   if ((cnf->fqdn = strdup(info->ai_canonname)) == NULL)
   {
      rgu_perror(cnf, "strdup()");
      freeaddrinfo(info);
      return(-1);
   };
   freeaddrinfo(info);

   if ((cnf->hostname = strdup(cnf->fqdn)) == NULL)
   {
      rgu_perror(cnf, "strdup()");
      return(-1);
   };

   if ((idx = index(cnf->hostname, '.')) != NULL)
      idx[0] = '\0';
   cidx = idx;
   if (cidx == NULL)
      cidx = ".local";
   if (cidx[1] == '\0')
      cidx = ".local";
   cidx++;

   if ((cnf->domainname = strdup(cidx)) == NULL)
   {
      rgu_perror(cnf, "strdup()");
      return(-1);
   };

   return(0);
}


int rgu_config_init(rgu_cnf ** cnfp, const char * prog_name)
{
   int          err;
   const char * ptr;

   assert(cnfp  != NULL);
   assert(*cnfp == NULL);
   rgu_version_assert();


   // allocate memory for configuration
   if ((*cnfp = malloc(sizeof(rgu_cnf))) == NULL)
   {
      rgu_perror(NULL, "malloc()");
      return(-1);
   };
   bzero(*cnfp, sizeof(rgu_cnf));


   // save program name
   if ((prog_name))
   {
      if ((ptr = rindex(prog_name, '/')) != NULL)
         if (ptr[1] != '\0')
            prog_name = &ptr[1];
      if (((*cnfp)->prog_name = strdup(prog_name)) == NULL)
      {
         rgu_perror(NULL, "strdup()");
         rgu_config_free(*cnfp);
         return(-1);
      };
   };


   // determines hostname of server
   if ((err = rgutil_config_hostname(*cnfp)) == -1)
   {
      rgu_config_free(*cnfp);
      return(-1);
   };

   return(0);
}


int rgu_config_parse(rgu_cnf * cnf)
{
   int                 err;
   struct rgu_file   * fs;
   const char        * path;

   assert(cnf != NULL);

   if ((path = cnf->cnffile) == NULL)
      path = PKGCONFDIR "/" PACKAGE_TARNAME ".conf";

   rgu_debug(cnf, RGU_DCONF, "opening file \"%s\"", path);
   fs = NULL;
   if ((err = rgu_fs_open(cnf, path, 0, &fs)) != 0)
   {
      rgu_fs_closestack(&fs);
      return(-1);
   };

   rgu_debug(cnf, RGU_DCONF, "closing file \"%s\"", path);
   rgu_fs_closestack(&fs);

   return(0);
}

/* end of source */
