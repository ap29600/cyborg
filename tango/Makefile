CC = gcc
CFLAGS = -Wall -Wmissing-prototypes -Wextra -std=c11 -pedantic -O3
LFLAGS = -x c -fPIC -c
BINARY_PATH = ~/.local/bin

default: bin/tango

clean:
	$(RM) bin/tango obj/*

install: bin/tango
	cp bin/tango $(BINARY_PATH)/tango

obj:
	mkdir -p obj

bin:
	mkdir -p bin

bin/tango: obj/tango.o obj/libargs.so obj/libstringview.so bin
	$(CC) $(CFLAGS) obj/tango.o -L./obj -lstringview -largs -o bin/tango 

obj/tango.o: src/tango.c obj
	$(CC) $(CFLAGS) -c src/tango.c -o obj/tango.o

obj/libargs.so: lib/args.h obj
	$(CC) $(CFLAGS) $(LFLAGS) -DARGS_IMPLEMENTATION lib/args.h -o obj/libargs.so

obj/libstringview.so: lib/stringview.h obj
	$(CC) $(CFLAGS) $(LFLAGS) -DSTRINGVIEW_IMPLEMENTATION lib/stringview.h -o obj/libstringview.so

