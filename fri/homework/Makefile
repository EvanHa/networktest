PWD=$(shell pwd)

CC=gcc
CFLAGS+=
LIBS+=-lpthread

STARGET = server
CTARGET = client

SRC_SERVER += server.c
SRC_CLIENT += client.c

default: server client

server:
	$(CC) -o $(STARGET) $(SRC_SERVER) $(LIBS)

client:
	$(CC) -o $(CTARGET) $(SRC_CLIENT) $(LIBS)

clean:
	rm -rf *.o
	rm -rf $(STARGET) $(CTARGET)

