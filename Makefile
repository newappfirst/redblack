CC = gcc
CXXFLAGS = -Wall
LDFLAGS = -Wall
TARGETS = main

all: $(TARGETS)

clean:
	rm -f *.o $(TARGETS)

main: main.o redblacktree.o node.o
main.o: main.c redblacktree.h
redblacktree.o: redblacktree.c redblacktree.h node.h
node.o: node.c node.h
