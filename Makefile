CC = gcc
CFLAGS = -Wall -Wextra -g

SRCS = mysh.c parser.c executor.c
OBJS = $(SRCS:.c=.o)
TARGET = mysh

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c mysh.h
	$(CC) $(CFLAGS) -c $< -o $@

# mysh: mysh.o parser.o executor.o
# 	$(CC) $(CFLAGS) -o mysh mysh.o parser.o executor.o

clean:
	rm -f $(OBJS) $(TARGET)