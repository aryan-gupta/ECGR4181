#! /bin/env python

import os
from random import randint
import subprocess

def mk_rand_struct(num, depth):
	if depth > 7:
		return

	for i in range(num):
		if randint(0, 4) == 0:
			create_directory("folder" + str(i), depth + 1)
		else:
			create_file("file" + str(i) + ".bin")

def create_directory(dir, depth):
	os.mkdir(dir) # create dir
	os.chdir(dir) # go in directory

	mk_rand_struct(randint(0, 50), depth)
	os.chdir("..")

def create_file(fname):
	size = randint(1, 10)
	r = subprocess.call(['dd', 'bs=10K', f'count={size}', 'if=/dev/urandom', f'of={fname}'])


if __name__ == '__main__':
	# os.mkdir("copy") # create our initial folder that we want to copy
	os.chdir("copy")

	mk_rand_struct(randint(40, 60), 0)

