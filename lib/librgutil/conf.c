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
#include <sys/stat.h>
#include <errno.h>

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


/////////////////
//             //
//  Functions  //
//             //
/////////////////
#ifdef __RACKGNOME_PMARK
#pragma mark - Functions
#endif

void rgutil_config_free(rgu_cnf * cnf)
{
   assert(cnf != NULL);

   rgutil_config_free_str(&cnf->prog_name);
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


int rgutil_config_init(rgu_cnf ** cnfp, const char * prog_name)
{
   const char * ptr;

   assert(cnfp  != NULL);
   assert(*cnfp == NULL);
   rgutil_version_assert();


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
         rgutil_config_free(*cnfp);
         return(-1);
      };
   };

   return(0);
}


int rgutil_config_parse(rgu_cnf * cnf)
{
   int                 err;
   struct rgu_file   * fs;
   const char        * path;

   assert(cnf != NULL);

   if ((path = cnf->cnffile) == NULL)
      path = PKGCONFDIR "/" PACKAGE_TARNAME ".conf";

   fs = NULL;
   if ((err = rgu_fs_open(cnf, path, 0, &fs)) != 0)
   {
      rgu_fs_closestack(cnf, &fs);
      return(-1);
   };

   rgu_fs_closestack(cnf, &fs);

   return(0);
}

/* end of source */
