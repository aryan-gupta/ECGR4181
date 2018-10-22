
#include <iostream>

#include "main.hpp"
#include "Cache.hpp"
#include "printer.hpp"

ptr_t Cache::getTag(ptr_t ptr) {
	return ptr >> mAddrInfo.Tag_Offset;
}

ptr_t Cache::getIndex(ptr_t ptr) {
	ptr &= mAddrInfo.Index_Mask; // remove high bits
	return ptr >> mAddrInfo.Index_Offset;
}

ptr_t Cache::getOffset(ptr_t ptr) {
	return ptr & mAddrInfo.Offset_Mask;
}

Cache::Cache(const ParseData& pd)
	: mAddrInfo{ get_info(pd) }, mCache{  }, mNext{  }, mHits{  }, mAccess{  }
{
	// The size of mCache must be size of the index bits, or
	// (actual cache size) / (cache block size)
	size_t iCacheSize = pd.cache_size / pd.block_size;
	mCache = new cache_info[iCacheSize];

	size_t associativeSize = iCacheSize / pd.associativity;

	mReplace = get_replace_func(pd.associativity, associativeSize);
	mLocate = get_locate_func(pd.associativity, associativeSize);
}

Cache::~Cache() {
	delete[] mCache;
}

void Cache::read(ptr_t addr) {
	ptr_t index = getIndex(addr);
	ptr_t tag   = getTag(addr);

	++mAccess;
	printer::dot();

	cache_info* loc = mCache + index;
	if (mLocate(loc, tag)) {
		++mHits; // We had a cache hit
		printer::bksp();
	} else {
		// if we had a miss check the next level cache
		if (mNext) mNext->read(addr);
		mReplace(loc, tag);
	}
}

void Cache::write(ptr_t addr) {
	read(addr); // a read is pretty much a write
}

void Cache::fetch(ptr_t addr) {
	// read(addr); // a fetch will basically do the same thing as a read
}

int Cache::getHits() const {
	return mHits;
}

int Cache::getAccess() const {
	return mAccess;
}

void always_replace_policy(cache_info* cache, ptr_t idx, ptr_t tag) {
	cache_info* loc = cache + idx;

	ASSERT(loc->tag != tag, "[E] Replacement policy `always_replace_policy' called on cache hit");

	loc->valid = true;
	loc->tag = tag;

}


locate_func_t get_locate_func(uint8_t assoc, size_t offset) {
	// if (assoc == 1) {
	// 	return [](cache_info* cache, ptr_t idx, ptr_t tag) -> bool {
	// 		cache_info* loc = cache + idx;
	// 		return loc->valid and loc->tag == tag;
	// 	};
	// }

	return [=](cache_info* loc, ptr_t tag) -> bool {
		for (int i = 0; i < assoc; ++i) {
			if (loc->valid and loc->tag == tag)
				return true;
			loc += offset;
		}
		return false;
	};

	/// if it is 4 ways assiciative when we want to divide the cache into 4 sections, check the index in the first
	/// section, if its not there check the next section, (wich will be at an offset of (cache size) / (assiciativity))
}

replace_func_t get_replace_func(uint8_t assoc, size_t offset) {
	uint8_t* least_used = new uint8_t[assoc]; // create bits for least_used
	std::fill(least_used, least_used + assoc, 0);

	return [=](cache_info* loc, ptr_t tag) -> void {
		static std::unique_ptr<uint8_t[]> dat{ least_used }; // assume ownership of least used (to prevent leakage)

		uint8_t& f = dat[0];
		uint8_t& s = dat[1];

		// decrement each one
		for (int i = 0; i < assoc; ++i) {
			if (dat[i]) --dat[i]; // decrement if not 0
		}

		// find the first one that is 0 and use it as cache
		for (int i = 0; i < assoc; ++i) {
			if (dat[i] == 0) {
				dat[i] = assoc;
				loc->valid = true;
				loc->tag = tag;
				break;
			} else {
				loc += offset;
			}
		}
	};
}