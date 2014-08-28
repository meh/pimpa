CC     = gcc
AR     = ar
CFLAGS = -Wall -Iinclude

SOURCES = source/helpers.c source/metadata.c source/plugin.c source/runtime.c
OBJECTS = $(SOURCES:.c=.o)
LIBRARY = libpimpa.a

all: $(SOURCES) $(LIBRARY)

$(LIBRARY) : $(OBJECTS)
	$(AR) rcs $(LIBRARY) $(OBJECTS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(LIBRARY)
