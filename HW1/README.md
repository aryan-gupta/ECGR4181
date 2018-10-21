Using the trace provided as input, use DineroIV to model an instruction cache and a data cache with a combined total cache space of 32KB (for a split cache, assume a 16KB instruction cache and a 16KB data cache. The block size should be varied (8B, 32B and 128B) and the associativity should be varied (direct mapped and 4-way). Model both split (separate instruction and data caches) and shared (all accesses go to a single cache that holds both instructions and data) caches. There are a total of 12 simulations. No other parameters should be varied. Graph the results you get from these experiments (cache hit rate and miss rate) and discuss in detail why you see different trends in the graphs (30pts).



32KB split (16KB, 16KB)
	- BlockSize:
		8B,
		32B,
		128B


split  8B   dm
            4w
       32B  dm
            4w
       128B dm
            4w
uni    8B   dm
            4w
       32B  dm
            4w
       128B dm
            4w