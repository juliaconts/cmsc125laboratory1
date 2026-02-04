CC = gcc
CFLAGS = -Wall -Wextra -g

all: mysh

mysh: mysh.o parser.o executor.o
	$(CC) $(CFLAGS) -o mysh mysh.o parser.o executor.o

clean:
	rm -f *.o mysh