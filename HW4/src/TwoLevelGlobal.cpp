

#include "main.hpp"
#include "TwoLevelGlobal.hpp"

bool BranchPredictorTypes::TwoLevelGlobal::operator()(addr_t addr) {
	addr_t idx = mGHT.value();
	return mPHT[idx].value() >= (counter_t::max() / 2);
}

void BranchPredictorTypes::TwoLevelGlobal::operator()(addr_t addr, bool taken, bool guess) {
	addr_t idx = mGHT.value();
	if (taken) {
		++mPHT[idx];
	} else {
		--mPHT[idx];
	}
	mGHT << taken;
}