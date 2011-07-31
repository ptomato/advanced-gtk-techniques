SUBDIRS = pixmaps data src po

desktopdir = $(datadir)/applications
dist_desktop_in_files = info-app.desktop.in
dist_desktop_DATA = $(dist_desktop_in_files:.desktop.in=.desktop)
@INTLTOOL_DESKTOP_RULE@

# Clean and ignore files
MAINTAINERCLEANFILES = .cvsignore ABOUT-NLS aclocal.m4 config.guess \
	config.rpath config.sub depcomp info-app.desktop install-sh missing \
	po/Makevars.template po/Rules-quot po/boldquot.sed po/en@boldquot.header \
	po/en@quot.header po/insert-header.sin po/quot.sed po/remove-potcdate.sin
GITIGNOREFILES = m4

-include $(top_srcdir)/git.mk
