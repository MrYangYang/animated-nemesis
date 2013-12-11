CC = gcc
CFLAGS = -Wall
DEBUG_FLAG = -g
LIBS = 

all: copy get

copy: copy.o libops.o
	${CC} $(CFLAGS) $(LIBS) copy.o libops.o -o copy

get: get.o libops.o
	${CC} $(CFLAGS) $(LIBS) get.o libops.o -o get

put:


clean:
	rm copy get put *.o

