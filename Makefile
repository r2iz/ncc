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
	./test/test.sh

donut: ncc
	./ncc test/donut.c > donut.s
	gcc -static -o donut donut.s
	./donut

donut-gcc: ncc
	gcc -static -o donut_gcc donut.s
	./donut_gcc


.PHONY: donut

.PHONY: all

clean:
	rm -f ncc *.o *~ tmp*

.PHONY: test clean
