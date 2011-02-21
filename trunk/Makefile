TARGET=proyecto_v1.0
CC=gcc
GLIB_CFLAGS = -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include/

CFLAGS+= -Wall -Wbad-function-cast -Wcast-qual -Wno-unused-parameter   -g  $(GLIB_CFLAGS) 
#falta -pedantic
LDFLAGS=-lglib-2.0 -lm
SOURCES=$(shell echo *.c)

OBJECTS=$(SOURCES:.c=.o)

all: $(TARGET)

# BString requiere flags distintos:
$(BSTRING_OBJECTS):CFLAGS=-ansi -Wall -Werror -pedantic -g

$(TARGET): $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS) 

clean:
	rm -f $(TARGET) $(OBJECTS) .depend *~



.depend: *.[ch]
	$(CC) -MM $(SOURCES) >.depend

-include .depend

.PHONY: clean all
