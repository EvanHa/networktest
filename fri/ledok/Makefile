obj-m := driver.o
KDIR  := /root/elayer/EmbeddedLinuxSystem/linux/kernel/linux-2.6.24-el2440-0504-final

all: driver.ko server client

driver.ko: driver.c
	make -C $(KDIR) SUBDIRS=$(PWD) modules

server: server.c
	arm-linux-gnu-gcc  server.c -o server

client: client.c
	gcc -Wall -O2 client.c -o client

clean:
	make -C $(KDIR) SUBDIRS=$(PWD) clean
	rm -f server client
