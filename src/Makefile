CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic -g
TARGETS=scanner.o parser.o main.o stack.o expression.o symtable.o


all: ifj

ifj: $(TARGETS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f *.o ifj

.PHONY: all clean
