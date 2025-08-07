CC = gcc
CFLAGS = -std=c11 -g -static -Wall -Wextra
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

ncc: $(OBJS)
	$(CC) $(CFLAGS) -o ncc $(OBJS)

%.o: %.c ncc.h
	$(CC) $(CFLAGS) -c $<

test: ncc
	./test.sh

clean:
	rm -f ncc *.o *~ tmp*

.PHONY: test clean
