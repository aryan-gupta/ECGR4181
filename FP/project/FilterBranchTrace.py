
import os

src = open("branches_0.out")
dest = open("rsync.trace", mode='w+')

for line in src:
	if line.split('\t')[2] == '1':
		dest.write(line)
