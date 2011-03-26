# Makefile for Advanced GTK Techniques

PAGES = \
	index.page \
	introduction.page \
	dynamic-actions.page \
	custom-container.page

%.page: %.page.in
	cp $< $@

all: $(PAGES)

.PHONY: all
