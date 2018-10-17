
#include <cmath>

#include "Simulator.hpp"

ptr_t Simulator::getTag(ptr_t ptr) {
	return ptr >> mAddrInfo.Tag_Offset;
}

ptr_t Simulator::getIndex(ptr_t ptr) {
	ptr &= mAddrInfo.Index_Mask; // remove high bits
	return ptr >> mAddrInfo.Index_Offset;
}

ptr_t Simulator::getOffset(ptr_t ptr) {
	return ptr & mAddrInfo.Offset_Mask;
}

Simulator::Simulator(std::vector<std::pair<Ops, ptr_t>>&& access, const ParseData& pd)
	: mAddrInfo{ get_info(pd) }, mCache{ nullptr }, mIns{ access }
{
	// The size of mCache must be size of the index bits, or
	// (actual cache size) / (cache block size)
	size_t iCacheSize = pd.cache_size / pd.block_size;
	mCache = new cache_info[iCacheSize];
}

Simulator::~Simulator() {
	delete[] mCache;
}

void Simulator::doSim() {
#if __cpp_structured_bindings
	for (auto [op, addr] : mIns) {
#else
	for (auto& _p : mIns) {
		auto op = _p.first; auto addr = _p.second;
#endif
		ptr_t index = getIndex(addr);
		ptr_t tag   = getTag(addr);

		++mAccess;

		cache_info* loc = mCache + index;
		if (loc->valid and loc->tag == tag) {
			// We had a cache hit
			++mHits;
		} else {
			// default replacement policy
			loc->valid = true;
			loc->tag = tag;
		}
	}
}