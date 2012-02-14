# Makefile for Advanced GTK Techniques

PAGES = \
	index.page \
	introduction.page \
	dynamic-actions.page \
	custom-container.page \
	real-life-app-setup.page \
	autoconf-project.page \
	automake-project.page \
	desktop-file.page \
	gettext-project.page \
	real-life-app-writing.page

all: $(PAGES)

%.page: %.page.in add-listings.py
	./add-listings.py $< >$@

autoconf-project.page: app-skeleton1/configure.ac

automake-project.page: app-skeleton2/configure.ac app-skeleton2/Makefile.am app-skeleton2/src/Makefile.am

desktop-file.page: app-skeleton3/app-skeleton.desktop app-skeleton3/Makefile.am app-skeleton3/pixmaps/Makefile.am

html: all
	mkdir -p html
	gnome-doc-tool html -o html --no-figures --custom-xslt=mallard-mathml.xslt *.page

hmtl: html

clean:
	rm -rf html

.PHONY: all html hmtl clean
