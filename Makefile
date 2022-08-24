CC = clang
CFLAGS = -O2 -pipe -std=c17 -Wall
AR = ar
ARFLAGS = rcs

.PHONY: clear

libargs.a: args.o
	$(AR) $(ARFLAGS) libargs.a args.o

args.o: args.c args.h
	$(CC) $(CFLAGS) -c args.c -o args.o

clear:
	rm -f *.o libargs.a
