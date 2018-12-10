
import os

src = open("rsync_final.trace")
dest = open("rsync_actual_final.trace", mode='w+')


taken = {}
ntake = {}

for line in src:
	# 0: pc address
	# 1: T/N
	# 2: target address
	split = line.split()
	if split[0] in taken:
		if split[1] == 'T':
			taken[split[0]] += 1
		else:
			ntake[split[0]] += 1
	else:
		taken[split[0]] = 0
		ntake[split[0]] = 0

src = open("rsync_final.trace")

for line in src:
	split = line.split()

	dest.write(line.rstrip('\n'))
	dest.write(' ')
	bias = taken[split[0]] > ntake[split[0]]
	dest.write('1' if bias else '0')
	dest.write("\n")

src.close()
dest.close()
