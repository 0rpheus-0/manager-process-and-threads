CC = gcc
CFLAGS = -W -Wall -Wno-unused-parameter -Wno-unused-variable -std=c11 -pedantic -g -g3
.PHONY:clean

all: main

run: main
	./main

main: main.c Makefile
	$(CC) $(CFLAGS) main.c -o main -lncurses

clean:
	rm -f main
