
#include "TwoBit.hpp"

bool BranchPredictorTypes::TwoBit::operator()(addr_t addr) {
	addr_t idx = get_sbits(addr);
	return mPHT[idx].value() >= (counter_t::max() / 2);
}

void BranchPredictorTypes::TwoBit::operator()(addr_t addr, bool taken, bool) {
	if (taken) {
		++mPHT[get_sbits(addr)];
	} else {
		--mPHT[get_sbits(addr)];
	}
}