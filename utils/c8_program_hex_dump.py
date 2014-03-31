#!/usr/bin/python3
# Reads in the bytes of a Chip 8 program, and then displays it's contents

import sys
import binascii

def main():
	if len(sys.argv) < 2:
		print('Need a file for the 1st arguement')
		exit()
	
	c8_file = sys.argv[1]
	c8 = open(c8_file, 'rb')
	c8_data = c8.read()
	
	for i in range(0, len(c8_data), 2):
		print('  %03X: '%(0x200 + i), end='')
		print('%02X %02X'%((c8_data[i]), (c8_data[i + 1])))

main()

