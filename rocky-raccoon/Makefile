PKG_CONFIG = gtk+-2.0 
SOURCES = rocky.c 
OBJECTS = rocky.o 

CFLAGS = -g -Wall -O0 `pkg-config --cflags $(PKG_CONFIG)`
LDFLAGS = `pkg-config --libs $(PKG_CONFIG)`

all: rocky-raccoon

rocky-raccoon: $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS)
	
clean:
	$(RM) $(OBJECTS) rocky-raccoon
	
