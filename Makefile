CC = gcc
CFLAGS = -std=c11 -g -static

ncc: main.c
	$(CC) $(CFLAGS) -o ncc main.c

test: ncc
	./test.sh

clean:
	rm -f ncc *.o *~ tmp*

.PHONY: test clean
