# EL-2440 kernel 2.6 driver Makefile

obj-m  := el2440_keybutton.o

PWD    := $(shell pwd)
KDIR   := $(PWD)/linux-2.6.24-el2440-v2.0

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
		
clean :	
	rm -rf *.ko
	rm -rf *.mod.*
	rm -rf .*.cmd
	rm -rf *.o


