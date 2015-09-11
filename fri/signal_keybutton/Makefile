# EL-2440 kernel 2.6 driver Makefile

obj-m  := el2440_key.o

KDIR  := /root/linux-2.6.24-el2440-v2.0

PWD    := $(shell pwd)

APP  = sig

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
		
	arm-linux-gnu-gcc -o $(APP) $(APP).c
	
clean :	
	rm -rf *.ko
	rm -rf *.mod.*
	rm -rf .*.cmd
	rm -rf *.o


