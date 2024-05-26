CC = gcc
CFLAGS = -W -Wall -Wno-unused-parameter -Wno-unused-variable -std=c11 -pedantic -g -g3 #-fsanitize=address
.PHONY:clean

all: main

run: main
	./main

main: main.c Makefile
	$(CC) $(CFLAGS) parser.c main.c -o main -lncurses

clean:
	rm -f main
