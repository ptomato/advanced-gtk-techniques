# Makefile for Advanced GTK Techniques

PAGES = \
	index.page \
	introduction.page \
	dynamic-actions.page \
	custom-container.page \
	real-life-app.page \
	autoconf-project.page \
	automake-project.page

all: $(PAGES)

%.page: %.page.in add-listings.py
	./add-listings.py $< >$@

autoconf-project.page: app-skeleton1/configure.ac

automake-project.page: app-skeleton2/configure.ac app-skeleton2/Makefile.am app-skeleton2/src/Makefile.am

html: all
	mkdir -p html
	cd html && \
	gnome-doc-tool html ../*.page

.PHONY: all html
