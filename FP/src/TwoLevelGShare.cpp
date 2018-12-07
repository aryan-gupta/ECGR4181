

#include "main.hpp"
#include "TwoLevelGShare.hpp"

bool BranchPredictorTypes::TwoLevelGShare::operator()(addr_t addr) {
	addr_t idx = get_sbits(addr) ^ mGHT.value();
	return mPHT[idx].value() >= (counter_t::max() / 2);
}

void BranchPredictorTypes::TwoLevelGShare::operator()(addr_t addr, bool taken, bool guess) {
	addr_t idx = get_sbits(addr) ^ mGHT.value();
	if (taken) {
		++mPHT[idx];
	} else {
		--mPHT[idx];
	}
	mGHT << taken;
}