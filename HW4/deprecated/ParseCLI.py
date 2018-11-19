
from ctypes import cdll
import platform

# if platform.system() == "Linux":
# 	clib = cdll.LoadLibrary("../bin/libparse.so")
# elif platform.system() == "Windows":
clib = cdll.LoadLibrary("Z:/Users/Nayra/Projects/ECGR4181/HW4/bin/libparse.dll")
# else:
# 	raise OSError("[E] Could not determine OS, please use Linux or Windows")


def get_parse(): # Super old fashion cli parsing
	pd = clib.parse(sys.argv, len(sys.argv))


