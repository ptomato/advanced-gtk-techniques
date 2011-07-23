### INITIALIZATION #############################################################
# In this section we do preparations for initializing our build system.

AC_INIT([App Skeleton], [2], [philip.chimento@gmail.com])
AC_CONFIG_SRCDIR([src/hello-world.c])
AM_INIT_AUTOMAKE([-Wall foreign])
AM_SILENT_RULES([yes])

### SHOPPING LIST ##############################################################
# In this section we notify autoconf of what tools we will be using.

AC_PROG_CC
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

AC_CONFIG_FILES([
	Makefile
	src/Makefile
])
AC_OUTPUT
