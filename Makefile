# Makefile for Advanced GTK Techniques

PAGES = \
	index.page \
	introduction.page \
	dynamic-actions.page \
	custom-container.page \
	skeleton-application.page

%.page: %.page.in add-listings.py
	./add-listings.py $< >$@

skeleton-application.page: app-skeleton1/configure.ac

all: $(PAGES)

html: all
	mkdir -p html
	cd html && \
	gnome-doc-tool html ../*.page

.PHONY: all html
