# EL-2440 kernel 2.6 driver Makefile

obj-m  := el2440_led.o

KDIR  := /root/elayer/linux/kernel/linux-2.6.24-el2440-0504-final
PWD    := $(shell pwd)

APP  = test

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
		
	arm-linux-gnu-gcc -o $(APP) $(APP).c
	
clean :	
	rm -rf *.ko
	rm -rf *.mod.*
	rm -rf .*.cmd
	rm -rf *.o


