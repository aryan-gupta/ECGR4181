

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

	mRPolicy = always_replace_policy;
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
	if (loc->valid and loc->tag == tag) {
		// We had a cache hit
		++mHits;
		printer::bksp();
	} else {
		// if we had a miss check the next level cache
		if (mNext)
			mNext->read(addr);
		// default replacement policy
		mRPolicy(mCache, index, tag);
	}
}

void Cache::write(ptr_t addr) {
	read(addr);
}

void Cache::fetch(ptr_t addr) {
	read(addr);
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
