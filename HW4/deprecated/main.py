import sys, os

from ParseCLI import get_parse
from reader import reader

def main():
	# data = get_parse()
	trace = reader(sys.argv[1])
	print(trace[0][0], end = " ")
	print(trace[0][1])
	print(trace[1][0], end = " ")
	print(trace[1][1])



if __name__ == '__main__':
	main()