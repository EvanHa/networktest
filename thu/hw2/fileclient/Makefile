CC = gcc
CFLAGS = -Wall

MYPROGS = $(patsubst %.c, %, $(wildcard *.c))

all: $(MYPROGS)

clean:
	rm -f $(MYPROGS)
