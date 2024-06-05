CC = gcc
CFLAGS = -W -Wall -Wno-unused-parameter -Wno-unused-variable -std=c11 -pedantic -g -g3 
.PHONY:clean

all: main

run:
	rm -f main; $(CC) $(CFLAGS) parser.c main.c -o main -lncurses; ./main

main: main.c Makefile
	$(CC) $(CFLAGS) parser.c main.c -o main -lncurses

clean:
	rm -f main
