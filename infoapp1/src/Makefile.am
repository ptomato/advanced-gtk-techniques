AM_CFLAGS = $(INFO_APP_CFLAGS)
AM_CPPFLAGS = -DLOCALEDIR=\""$(localedir)"\"
bin_PROGRAMS = info-app
info_app_SOURCES = main.c \
	pinfoapp.c pinfoapp.h
info_app_LDADD = $(INFO_APP_LIBS)


-include $(top_srcdir)/git.mk
