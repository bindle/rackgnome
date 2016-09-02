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
#define __MAIN_C 1
#undef __RACKGNOME_PMARK


///////////////
//           //
//  Headers  //
//           //
///////////////
#ifdef __RACKGNOME_PMARK
#pragma mark - Headers
#endif

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <getopt.h>

#include <rackgnome.h>
#include <rgutil.h>


///////////////////
//               //
//  Definitions  //
//               //
///////////////////
#ifdef __RACKGNOME_PMARK
#pragma mark - Definitions
#endif

#ifndef PROGRAM_NAME
#define PROGRAM_NAME "rackgnome"
#endif


//////////////////
//              //
//  Prototypes  //
//              //
//////////////////
#ifdef __RACKGNOME_PMARK
#pragma mark - Prototypes
#endif

int main(int argc, char * argv[]);

void usage(void);


/////////////////
//             //
//  Functions  //
//             //
/////////////////
#ifdef __RACKGNOME_PMARK
#pragma mark - Functions
#endif

int main(int argc, char * argv[])
{
   int                    c;
   int                    err;
   int                    opt_index;
   rgu_cnf              * cnf;

   static char          short_opt[] = "c:d:hTVvq+";
   static struct option long_opt[] =
   {
      { "help",          no_argument, 0, 'h'},
      { "version",       no_argument, 0, 'V'},
      { "version-terse", no_argument, 0, 'V'},
      { NULL,            0,           0, 0  }
   };


   // initialize Rack Gnome state
   cnf = NULL;
   if ((err = rgu_config_init(&cnf, argv[0])) == -1)
      return(1);


   while((c = getopt_long(argc, argv, short_opt, long_opt, &opt_index)) != -1)
   {
      switch(c)
      {
         case -1:	/* no more arguments */
         case 0:	/* long options toggles */
         break;

         case 'd':
         cnf->debug = strtoumax(optarg, NULL, 0);
         break;

         case 'c':
         if ((cnf->cnffile))
            free(cnf->cnffile);
         if ((cnf->cnffile = strdup(optarg)) == NULL)
         {
            fprintf(stderr, "%s: out of virtual memory\n", cnf->prog_name);
            rgu_config_free(cnf);
            return(1);
         };
         break;

         case 'h':
         usage();
         return(0);

         case 'T':
         rgu_version_print_terse(cnf->prog_name);;
         return(0);

         case 'V':
         rgu_version_print(cnf->prog_name);
         return(0);

         case '?':
         fprintf(stderr, "Try `%s --help' for more information.\n", cnf->prog_name);
         rgu_config_free(cnf);
         return(1);

         default:
         fprintf(stderr, "%s: unrecognized option `--%c'\n", cnf->prog_name, c);
         fprintf(stderr, "Try `%s --help' for more information.\n", cnf->prog_name);
         rgu_config_free(cnf);
         return(1);
      };
   };

   if ((err = rgu_config_parse(cnf)) != 0)
   {
      rgu_config_free(cnf);
      return(1);
   };

   rgu_config_free(cnf);

   return(0);
}


void usage(void)
{
   printf("Usage: %s [OPTIONS] command [OPTIONS]\n", PROGRAM_NAME);
   printf("Common Options:\n");
   printf("  -c file                   configuration file\n");
   printf("  -d level                  set debug level\n");
   printf("  -h, --help                print this help and exit\n");
   printf("  -V, --version             print version number and exit\n");
   printf("  -v, --verbose             print verbose messages\n");
   printf("  -T, --version-terse       print version number and exit\n");
   printf("  -q, --quiet, --silent     do not print messages\n");
   printf("Commands:\n");
   printf("  configtest                displays version information\n");
   printf("  version                   displays version information\n");
   printf("\n");
   return;
}

/* end of source */
