CC = gcc
CFLAGS = -std=c11 -g -static -Wall -Wextra
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
NCC=./ncc

ncc: $(OBJS)
	$(CC) $(CFLAGS) -o ncc $(OBJS)

%.o: %.c ncc.h
	$(CC) $(CFLAGS) -c $<

test: ncc
	./test.sh

test-c: ncc
	./ncc test/test.c > tmp-c.s
	gcc -static -o tmp-c tmp-c.s
	./tmp-c


clean:
	rm -f ncc *.o *~ tmp*

.PHONY: test clean
