# Makefile for Advanced GTK Techniques

PAGES = \
	index.page \
	introduction.page \
	dynamic-actions.page \
	custom-container.page \
	real-life-app.page \
	autoconf-project.page \
	automake-project.page

%.page: %.page.in add-listings.py
	./add-listings.py $< >$@

autoconf-project.page: app-skeleton1/configure.ac

all: $(PAGES)

html: all
	mkdir -p html
	cd html && \
	gnome-doc-tool html ../*.page

.PHONY: all html
