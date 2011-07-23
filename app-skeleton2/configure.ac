### INITIALIZATION #############################################################
# In this section we do preparations for initializing our build system.

# Initialize autoconf.
# The arguments are:
#  - human-readable package name
#  - version of the package
#  - address for bug reports
#  - tarball name
AC_INIT([App Skeleton], [2], [philip.chimento@gmail.com])

# Sanity check to make sure we are running autoconf from the right directory
AC_CONFIG_SRCDIR([src/hello-world.c])

# Initialize automake, and tell it to warn of any improprieties
AM_INIT_AUTOMAKE([-Wall foreign])

# Disable long swathes of garbage scrolling across our terminal
# AM_SILENT_RULES([yes])

### SHOPPING LIST ##############################################################
# In this section we notify autoconf of what tools we will be using.

# First off, we require a C compiler
AC_PROG_CC

# We need 'pkg-config' for checking for the proper versions of the libraries we
# use, and for discovering their compiler and linker flags
PKG_PROG_PKG_CONFIG

### LIBRARIES ##################################################################
# In this section we use pkg-config to configure the libraries we are using.

PKG_CHECK_MODULES([APP_SKELETON], [
	glib-2.0
	gtk+-2.0
])

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
])

# Do all the output we have requested
AC_OUTPUT
