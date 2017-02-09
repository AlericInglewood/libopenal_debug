# Define package version here.
#
# CW_VERSION_MAJOR               : Increment when the whole library was rewritten; or some VERY MAJOR change was made.
# CW_VERSION_MINOR               : Increment when major changes have occured, like removing interfaces.
# CW_VERSION_REVISION            : Increment every public release; or set to 0 when CW_VERSION_MINOR was incremented.

define(CW_VERSION_MAJOR, 0)
define(CW_VERSION_MINOR, 0)
define(CW_VERSION_REVISION, 0)

# Package name and bug reporting.
define(CW_PACKAGE_NAME, [libopenal_debug])
define(CW_BUGREPORT, [Aleric.Inglewood@gmail.com])

# Version info, see http://www.gnu.org/software/libtool/manual.html#Updating-version-info
#
# CW_INTERFACE_VERSION           : Current interface version, increment whenever the interface is changed.
# CW_INTERFACE_VERSION_REVISION  : Revision, increment every public release; or set to 0 when the interface is changed.
# CW_INTERFACE_AGE               : Increment if interfaces have been added; or set to 0 when interfaces have been removed.

define(CW_INTERFACE_VERSION, 0)
define(CW_INTERFACE_VERSION_REVISION, 1)
define(CW_INTERFACE_AGE, 0)

# CW_COMPILE_FLAGS may contain warning flags, but also default debug flags (-g*)
# and optimization flags (-O*) that which will be stripped when not required.
define(CW_COMPILE_FLAGS, [-W -Wall -Wundef -Wpointer-arith -Wwrite-strings -Winline])
# CW_THREADS can be [no] (single-threaded), [yes] (multi-threaded) or [both] (single and multi-threaded applications).
define(CW_THREADS, [both])

# Include cwm4 header.
m4_include([cwm4/configure_ac_top.m4])

# These CFLAGS are used in the .pc file.
PCCFLAGS=
AC_SUBST(PCCFLAGS)

# Detect libbfd
AC_CHECK_LIB(bfd, bfd_init, [LIBBFD=-lbfd])
AC_CHECK_LIB(readline, readline, [LIBREADLINE=-lreadline])

AC_SUBST(LIBBFD)
AC_SUBST(LIBREADLINE)

AC_CONFIG_FILES([src/Makefile] [libopenal_debug.pc])

# Include cwm4 footer.
m4_include([cwm4/configure_ac_bottom.m4])
