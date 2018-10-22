
#include <cmath>

#include "Simulator.hpp"
#include "printer.hpp"


Simulator::Simulator(access_type&& access, const ParseData& pd)
	: mDataCache{  }, mInsCache{  }, mIns{ access }
{
	mDataCache = new Cache{ pd };
	mInsCache = mDataCache; // for now
}

Simulator::~Simulator() {
	if (mDataCache == mInsCache) {
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