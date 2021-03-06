#
#   Rack Gnome
#   Copyright (C) 2016 David M. Syzdek <syzdek@bindlebinaries.com>.
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
#   @configure_input@
#
#   Makefile.am - automate compiling on a unix platform
#

# Sub directories
SUBDIRS					=
DIST_SUBDIRS				=


# Global flags
AM_LIBS					=
AM_LDFLAGS				=
AM_CFLAGS				= -O2 $(CFLAGS_WARNINGS)
AM_CXXFLAGS				= -O2 @AM_CXXFLAGS@
AM_OBJCFLAGS				= -O2 @AM_OBJCFLAGS@
AM_CPPFLAGS				= -O2 -UPMARK -DHAVE_CONFIG_H=1 \
					  -I$(top_builddir)/include \
					  -I$(top_srcdir)/include \
					  -DSYSCONFDIR="\"$(sysconfdir)\""
ACLOCAL_AMFLAGS				= -I m4 -W all -W error
AM_MAKEINFOFLAGS                        = --no-split
DEFS					=
LDADD					= $(lib_LTLIBRARIES) \
					  $(noinst_LTLIBRARIES) \
					  $(noinst_LIBRARIES)


# automake targets
check_PROGRAMS				=
doc_DATA				= AUTHORS \
					  COPYING \
					  ChangeLog \
					  INSTALL \
					  NEWS \
					  README
include_HEADERS				= include/rackgnome.h
lib_LTLIBRARIES				= lib/librackgnome.la
man_MANS				=
info_TEXINFOS				=
noinst_LTLIBRARIES			=
noinst_LIBRARIES			= lib/librgutil.a
noinst_HEADERS				= include/rgutil.h
noinst_PROGRAMS				=
bin_PROGRAMS				= src/rackgnome-version
bin_SCRIPTS				=
pkgdata_DATA				=
sbin_SCRIPTS				=


# lists
AM_INSTALLCHECK_STD_OPTIONS_EXEMPT	=
BUILT_SOURCES				=
TESTS					=
XFAIL_TESTS				=
EXTRA_MANS				=
EXTRA_DIST				= lib/librackgnome/librackgnome.sym \
					  lib/librgutil/librgutil.sym
CLEANFILES				= \
					  $(builddir)/a.out   $(srcdir)/a.out \
					  $(builddir)/*/a.out $(srcdir)/*/a.out \
					  config.h.in~ $(srcdir)/config.h.in~ \
					  $(man_MANS) \
					  $(pkgdata_DATA) \
					  $(bin_SCRIPTS) \
					  $(sbin_SCRIPTS) \
					  @PACKAGE_TARNAME@-*.tar.* \
					  @PACKAGE_TARNAME@-*.txz \
					  @PACKAGE_TARNAME@-*.zip
DISTCHECK_CONFIGURE_FLAGS		= --enable-strictwarnings


# macros for lib/librackgnome.la
lib_librackgnome_la_DEPENDENCIES	= Makefile lib/librackgnome/librackgnome.sym config.h
lib_librackgnome_la_LDFLAGS		= -rpath'$(libdir)'\
					  -version-info $(LIB_VERSION_INFO) \
					  $(AM_LDFLAGS) \
					  -export-symbols $(srcdir)/lib/librackgnome/librackgnome.sym
lib_librackgnome_la_SOURCES		= \
					  include/rackgnome.h \
					  lib/librackgnome/librackgnome.h \
					  lib/librackgnome/version.c \
					  lib/librackgnome/version.h


# macros for lib/librgutil.a
lib_librgutil_a_DEPENDENCIES		= Makefile lib/librgutil/librgutil.sym config.h
lib_librgutil_a_LDFLAGS			= -rpath'$(libdir)'\
					  -version-info $(LIB_VERSION_INFO) \
					  $(AM_LDFLAGS) \
					  -export-symbols $(srcdir)/lib/librgutil/librgutil.sym
lib_librgutil_a_SOURCES			= \
					  include/rgutil.h \
					  lib/librgutil/librgutil.h \
					  lib/librgutil/daemon.c \
					  lib/librgutil/daemon.h \
					  lib/librgutil/timer.c \
					  lib/librgutil/timer.h \
					  lib/librgutil/version.c \
					  lib/librgutil/version.h


# macros for src/rackgnome-version
src_rackgnome_version_DEPENDENCIES	= Makefile \
					  $(noinst_LIBRARIES)
src_rackgnome_version_SOURCES		= $(include_HEADERS) \
					  $(noinst_HEADERS) \
					  src/rackgnome-version.c


# Makefile includes
GIT_PACKAGE_VERSION_DIR=include
SUBST_EXPRESSIONS =
include $(srcdir)/@bindletools_srcdir@/build-aux/makefile-autotools.am
include $(srcdir)/@bindletools_srcdir@/build-aux/makefile-subst.am
include $(srcdir)/@bindletools_srcdir@/build-aux/makefile-version.am


# custom targets
.PHONY:


# local targets
install-exec-local:

install-data-local:

install-data-hook:

uninstall-local:

clean-local:

distclean-local:
	rm -fR $(srcdir)/autom4te.cache

# end of automake file
