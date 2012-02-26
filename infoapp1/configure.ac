### INITIALIZATION #############################################################
# In this section we do preparations for initializing our build system.

# Initialize autoconf.
# The arguments are:
#  - human-readable package name
#  - version of the package
#  - address for bug reports
#  - tarball name
AC_INIT([Info App], [1], [philip.chimento@gmail.com])

# Sanity check to make sure we are running autoconf from the right directory
AC_CONFIG_SRCDIR([src/main.c])

# Initialize automake, and tell it to warn of any improprieties
AM_INIT_AUTOMAKE([-Wall foreign])

# Disable long swathes of garbage scrolling across our terminal
AM_SILENT_RULES([yes])

### SHOPPING LIST ##############################################################
# In this section we notify autoconf of what tools we will be using.

# First off, we require a C compiler
AC_PROG_CC

# We need 'pkg-config' for checking for the proper versions of the libraries we
# use, and for discovering their compiler and linker flags
PKG_PROG_PKG_CONFIG

# We use 'gettext' for internationalizing the program source code...
AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.18.1])

# ...and 'intltool' for internationalizing the data files.
IT_PROG_INTLTOOL([0.40])

### LIBRARIES ##################################################################
# In this section we use pkg-config to configure the libraries we are using.

PKG_CHECK_MODULES([INFO_APP], [
	glib-2.0
	gio-2.0 >= 2.28
	gtk+-3.0
])
# Check for OS X integration package
PKG_CHECK_EXISTS([gtk-mac-integration], [
	PKG_CHECK_MODULES([GTK_MAC_INTEGRATION], [gtk-mac-integration])
	INFO_APP_CFLAGS="$INFO_APP_CFLAGS $GTK_MAC_INTEGRATION_CFLAGS"
	INFO_APP_LIBS="$INFO_APP_LIBS $GTK_MAC_INTEGRATION_LIBS"
	AC_SUBST([INFO_APP_CFLAGS])
	AC_SUBST([INFO_APP_LIBS])
])

### VARIABLES ##################################################################
# Here, we deal with optional packages and substitution variables.

# The GETTEXT_PACKAGE variable is needed by intltool. We set it to the package's
# 'tarball name'.
GETTEXT_PACKAGE=${PACKAGE_TARNAME}
AC_SUBST([GETTEXT_PACKAGE])

### OUTPUT #####################################################################
# After checking all this stuff, we must output our results to the rest of the
# build system. That is what this section does.

# List the files we would like to output. If we put a file 'plerp' in this list,
# then autoconf will look for a file 'plerp.in' and transform it into 'plerp' by
# replacing any @VARIABLE@ strings with the contents of VARIABLE. We have not
# defined any of these variables explicitly yet, but they are defined internally
# by many of the macros we have used.
AC_CONFIG_FILES([
	Makefile
	src/Makefile
	data/Makefile
	pixmaps/Makefile
	po/Makefile.in
])

# Do all the output we have requested
AC_OUTPUT
