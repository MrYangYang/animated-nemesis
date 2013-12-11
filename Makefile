CC = gcc
CFLAGS = -Wall
DEBUG_FLAG = -g
LIBS = 

all: copy get put main

copy: copy.o libops.o
	${CC} $(CFLAGS) $(LIBS) copy.o libops.o -o copy

get: get.o libops.o
	${CC} $(CFLAGS) $(LIBS) get.o libops.o -o get

put: put.o libops.o
	${CC} $(CFLAGS) $(LIBS) put.o libops.o -o put

main: main.o libops.o
	${CC} $(CFLAGS) $(LIBS) main.o libops.o -o main 

clean:
	rm copy get put main *.o

