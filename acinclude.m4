#
#   Rack Gnome
#   Copyright (C) 2016 David M. Syzdek <david@syzdek.net>.
#
#   @SYZDEK_BSD_LICENSE_START@
#
#   Redistribution and use in source and binary forms, with or without
#   modification, are permitted provided that the following conditions are
#   met:
#
#      * Redistributions of source code must retain the above copyright
#        notice, this list of conditions and the following disclaimer.
#      * Redistributions in binary form must reproduce the above copyright
#        notice, this list of conditions and the following disclaimer in the
#        documentation and/or other materials provided with the distribution.
#      * Neither the name of David M. Syzdek nor the
#        names of its contributors may be used to endorse or promote products
#        derived from this software without specific prior written permission.
#
#   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
#   IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
#   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
#   PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL DAVID M. SYZDEK BE LIABLE FOR
#   ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
#   DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
#   SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
#   CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
#   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
#   OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
#   SUCH DAMAGE.
#
#   @SYZDEK_BSD_LICENSE_END@
#
#   acinclude.m4 - custom m4 macros used by configure.ac
#

# AC_RACKGNOME_COMPONENTS
# ______________________________________________________________________________
AC_DEFUN([AC_RACKGNOME_COMPONENTS],[dnl

   enableval=""
   AC_ARG_ENABLE(
      doxygen-docs,
      [AS_HELP_STRING([--enable-doxygen-docs], [enable Doxygen documentation])],
      [ EDOXYGEN=$enableval ],
      [ EDOXYGEN=$enableval ]
   )
   enableval=""
   AC_ARG_ENABLE(
      librackgnome,
      [AS_HELP_STRING([--enable-librackgnome], [enable building Rack Gnome client library])],
      [ ELIBRACKGNOME=$enableval ],
      [ ELIBRACKGNOME=$enableval ]
   )


   ENABLE_DOXYGEN=no
   ENABLE_LIBRACKGNOME=yes
   ENABLE_LIBRGUTIL=no


   if test "x${EDOXYGEN}" = "xyes";then
      ENABLE_DOXYGEN=yes
   fi
   if test "x${ELIBRACKGNOME}" = "xno";then
      ENABLE_LIBRACKGNOME=no
   fi
   if test "x${ENABLE_LIBRACKGNOME}" = "xyes";then
      ENABLE_LIBRGUTIL=yes
   fi


   AM_CONDITIONAL([ENABLE_DOXYGEN],       [test "${ENABLE_DOXYGEN}"        == "yes"])
   AM_CONDITIONAL([ELIBRACKGNOME],        [test "${ENABLE_LIBRACKGNOME}"   == "yes"])
   AM_CONDITIONAL([ENABLE_LIBRGUTIL],     [test "${ENABLE_LIBRGUTIL}"      == "yes"])
])dnl


# end of m4 file
