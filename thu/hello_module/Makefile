# EL-2440 kernel 2.6 driver Makefile

obj-m  := hello_module.o

KDIR  := /root/elayer/linux/kernel/linux-2.6.24-el2440-0504-final
PWD    := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
		
clean :	
	rm -rf *.ko
	rm -rf *.mod.*
	rm -rf .*.cmd
	rm -rf *.o


