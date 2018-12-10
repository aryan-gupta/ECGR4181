#!/bin/env python

import subprocess
import os

BaseOptions = ["GShare", "Global", "TwoBit", "Local"] # --agree-base=
AgreeOptions = ["AgreeBTFN", "AgreeFirst", "AgreeISA"] # -p
SigBits = "10-17" # --shiftreg-bits --sig-bits
LCOBits = "0, 7, 14" # --lco-bits

#bin/main.out -fproject/rsync_actual_final.trace -pAgreeI --agree-base=Global
print(os.getcwd())

for base in BaseOptions:
	for agree in AgreeOptions:
		for sig in range(10, 17):
			for lco in [0, 7, 14]:
				cmd = ["./bin/main.out", "-fproject/rsync_actual_final.trace", f"-p{agree}", f"--agree-base={base}", f"--shiftreg-bits={sig}", f"--sig-bits={sig}", f"--sig-lco-bits={lco}"]
				print(cmd, end='\t')
				out = subprocess.check_output(cmd, encoding='UTF-8')
				fname = "results/" + str(agree) + "-" + str(base) + "-" + str(sig) + "-" + str(lco) + ".txt"
				print(fname)
				dest = open(fname , mode="w+")
				dest.write(str(out))
