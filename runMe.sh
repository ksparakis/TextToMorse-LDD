#!/bin/sh
rm /dev/t2Morse
mknod /dev/t2Morse c 60 0
insmod km/t2Morse.ko
