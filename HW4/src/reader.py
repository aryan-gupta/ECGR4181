import os

def reader(filename):
	if not filename:
		raise ValueError("[E] filename cannot be empty")

	if not os.path.isfile(filename):
		raise ValueError(f"[E] filename `{filename}' is not valid")

	trace = []

	f = open(filename)
	for line in f:
		parts = line.split()
		addr = int(parts[0])
		branch = True if parts[1] == 'T' else False
		trace.append((addr, branch))

	return trace
