## Process this file with automake to generate Makefile.in

AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = @ACLOCAL_CWFLAGS@

SUBDIRS = src .

EXTRA_DIST = LICENSE.GPL3 INSTALL README NEWS
DISTCLEANFILES = stamp-h.in

DEFS = @DEFS@
CFLAGS = @CFLAGS@
LIBS =

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libopenal_debug.pc

# --------------- Maintainer's Section

distclean-local:
	rm -rf $(srcdir)/autom4te.cache *~

MAINTAINERCLEANFILES = $(srcdir)/aclocal.m4 $(srcdir)/config.h.in $(srcdir)/configure $(srcdir)/Makefile.in \
	$(srcdir)/install-sh $(srcdir)/missing $(srcdir)/compile $(srcdir)/mkinstalldirs $(srcdir)/config.guess \
	$(srcdir)/config.sub $(srcdir)/ltmain.sh $(srcdir)/stamp-h.in $(srcdir)/ChangeLog $(srcdir)/depcomp troep*
