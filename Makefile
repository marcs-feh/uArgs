CC = clang
CFLAGS = -O2 -pipe -std=c17 -Wall -g
AR = ar
ARFLAGS = rcs

.PHONY: clear

libuargs.a: uargs.o
	$(AR) $(ARFLAGS) libuargs.a uargs.o

uargs.o: uargs.c uargs.h
	$(CC) $(CFLAGS) -c uargs.c -o uargs.o

clean:
	rm -f *.o libuargs.a
