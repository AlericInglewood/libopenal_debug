include $(srcdir)/cwm4/root_makefile_top.am

SUBDIRS = @CW_SUBDIRS@ src

EXTRA_DIST += LICENSE.GPL3 INSTALL README NEWS
DISTCLEANFILES = stamp-h.in

CFLAGS = @CFLAGS@
LIBS =

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libopenal_debug.pc

include $(srcdir)/cwm4/root_makefile_bottom.am
