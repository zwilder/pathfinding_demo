CC = gcc

CFLAGS = -lncurses -lm -I./include/

OFLAGS = -O2

GFLAGS = -g -Wall

SOURCES = ./src/*.c

all: Pathfinding

Pathfinding: ctags
	$(CC) $(SOURCES) $(CFLAGS) $(GFLAGS) -o Pathfinding 

run:
	./Pathfinding

clean:
	rm Pathfinding

fresh: clean Pathfinding run

optimized:
	$(CC) $(SOURCES) $(CFLAGS) $(OFLAGS) -o Pathfinding

ctags: 
	ctags -R *
