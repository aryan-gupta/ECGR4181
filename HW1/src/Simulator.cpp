
#include <cmath>

#include "Simulator.hpp"
#include "printer.hpp"


Simulator::Simulator(access_type&& access, const ParseData& pd)
	: mDataCache{  }, mInsCache{  }, mIns{ access }
{
	if (pd.uni) {
		mDataCache = new Cache{ pd.uni_cache_size, pd.uni_block_size, pd.uni_associativity };
		mInsCache = mDataCache; // If our cache is unified then the InsCache is the same as DataCache
	} else {
		mDataCache = new Cache{ pd.dat_cache_size, pd.dat_block_size, pd.dat_associativity };
		mInsCache = new Cache{ pd.ins_cache_size, pd.ins_block_size, pd.ins_associativity };
	}
}

Simulator::~Simulator() {
	if (mDataCache == mInsCache) { // if we have an unified cache then just delete using one ptr
		delete mDataCache;
	} else {
		delete mInsCache;
		delete mDataCache;
	}
}

void Simulator::simulate() {
#if __cpp_structured_bindings
	for (auto [op, addr] : mIns) {
#else
	for (auto& _p : mIns) {
		auto op = _p.first; auto addr = _p.second;
#endif
		switch(op) {
			case Ops::FETCH: mInsCache->fetch(addr); break;
			case Ops::READ : mDataCache->read(addr); break;
			case Ops::WRITE: mDataCache->write(addr); break;
		}
	}
}

ratio_t<int> Simulator::getRatio() const {
	return { mDataCache->getHits(), mDataCache->getAccess() };
}