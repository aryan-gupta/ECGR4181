#!/bin/env python

import subprocess
import os
from threading import Thread

BaseOptions = ["GShare", "Global", "TwoBit", "Local"] # --agree-base=
AgreeOptions = ["null"] #["AgreeBTFN", "AgreeFirst", "AgreeISA"] # -p
SigBits = "10-17" # --shiftreg-bits --sig-bits
LCOBits = "0, 7, 14" # --lco-bits


#bin/main.out -fproject/rsync_actual_final.trace -pAgreeI --agree-base=Global
print(os.getcwd())

   
def mt_base_func(base):
	for agree in AgreeOptions:
		for sig in range(10, 17):
			for lco in [0, 7, 14]:
				cmd = ["./bin/main.out", "-fproject/rsync_actual_final.trace", f"-p{base}", f"--shiftreg-bits={sig}", f"--sig-bits={sig}", f"--sig-lco-bits={lco}"]
				print(cmd, end='\t')
				out = subprocess.check_output(cmd, encoding='UTF-8')
				fname = "results_base/" + str(agree) + "-" + str(base) + "-" + str(sig) + "-" + str(lco) + ".txt"
				print(fname)
				dest = open(fname , mode="w+")
				dest.write(str(out))
	print("done")

threads = []
for base in BaseOptions:
	t = Thread(target=mt_base_func, args=(base, ))
	threads.append(t)
	t.start()

while True: # dont kill parent thread
   pass
   