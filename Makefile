CC     = gcc
AR     = ar
CFLAGS = -fPIC -DXP_UNIX=1 -g

SOURCES = source/helpers.c source/plugin.c source/runtime.c
OBJECTS = $(SOURCES:.c=.o)
LIBRARY = libpimpa.a
TEST    = examples/nptest.so

all: $(SOURCES) $(LIBRARY)

$(LIBRARY) : $(OBJECTS)
	$(AR) rcs $(LIBRARY) $(OBJECTS)

.c.o:
	$(CC) -Wall -Iinclude $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(LIBRARY) examples/nptest.so

test: $(TEST)

$(TEST) : $(LIBRARY)
	$(CC) -Wall -Iinclude $(CFLAGS) -shared -o examples/nptest.so examples/test.c $(LIBRARY)
