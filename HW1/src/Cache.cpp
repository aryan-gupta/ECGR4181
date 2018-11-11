
#include <iostream>
#include <cstring>

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

Cache::Cache(uint32_t cache, uint32_t block, uint8_t assoc)
	: mAddrInfo{ get_info(cache, block, assoc) }, mCache{  }, mNext{  }, mHits{  }, mAccess{  }
{
	// The size of mCache must be size of the index bits, or
	// (actual cache size) / (cache block size)
	size_t iCacheSize = cache / block;
	mCache = new cache_info[iCacheSize];

	std::memset(mCache, 0, sizeof(cache_info) * iCacheSize); // cache_info is trivial so this works

	size_t assocCacheSize = iCacheSize / assoc;

	mReplace = get_replace_func(assoc, assocCacheSize);
	mLocate = get_locate_func(assoc, assocCacheSize);
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
	read(addr); // a fetch will basically do the same thing as a read
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
	/// if it is 4 ways assiciative when we want to divide the cache into 4 sections, check the index in the first
	/// section, if its not there check the next section, (wich will be at an offset of (cache size) / (assiciativity))

	return [=](cache_info* loc, ptr_t tag) -> bool {
		bool found = false;

		for (int i = 0; i < assoc; ++i) {
			if (!found and loc->valid and loc->tag == tag) {
				loc->lru = assoc - 1;
				found = true;
			} else {
				if (loc->lru) --(loc->lru);
			}
			loc += offset;
		}
		return found;
	};
}

replace_func_t get_replace_func(uint8_t assoc, size_t offset) {
	return [=](cache_info* loc, ptr_t tag) -> void {
		// find the first one that is 0 and use it as cache
		bool replaced = false;
		for (int i = 0; i < assoc; ++i) {
			if (!replaced and loc->lru == 0) {
				loc->lru = assoc - 1;
				loc->valid = true;
				loc->tag = tag;
				replaced = true;
			} else {
				if (loc->lru) --(loc->lru);
			}
			loc += offset;
		}
	};
}