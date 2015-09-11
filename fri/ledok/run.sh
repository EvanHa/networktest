#!/bin/sh

rmmod driver

if [ ! -e /dev/led ]; then
	mknod /dev/led c 240 0
fi

insmod driver.ko

./server 9000

exit 0
