
import os

src = open("rsync.trace")
dest = open("rsync_final.trace", mode='w+')

for line in src:
	split = line.split('\t')
	target = split[0]
	taken = 'T' if split[1] == '1' else 'N'
	pc_val = split[-1][split[-1].find('0x'):split[-1].find('#')]
	dest.write(pc_val.replace("0x", "").strip())
	dest.write(' ')
	dest.write(taken)
	dest.write(' ')
	dest.write(target.replace("0x", "").lstrip("0").strip())
	dest.write('\n')

#Gshare Global Local TwoBit
# 0, 7 and 14 offset

# Sorting the addresses