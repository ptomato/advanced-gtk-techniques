PKG_CONFIG = gtk+-2.0 
SOURCES = rocky.c 
OBJECTS = rocky.o 

CFLAGS = -g -Wall -O0 `pkg-config --cflags $(PKG_CONFIG)`
LDFLAGS = `pkg-config --libs $(PKG_CONFIG)`

all: rocky-raccoon rocky-interface.xml rocky-menus.xml

rocky-raccoon: $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS)

rocky-interface.xml: rocky-interface.glade
	gtk-builder-convert $< $@
	
clean:
	$(RM) $(OBJECTS) rocky-raccoon rocky-interface.xml
	
