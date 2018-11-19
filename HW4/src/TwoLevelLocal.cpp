

#include "main.hpp"
#include "TwoLevelLocal.hpp"

bool BranchPredictorTypes::TwoLevelLocal::operator()(addr_t addr) {
	addr_t idx = mLHR[get_sbits<7>(addr)].value();
	return mPHT[idx].value() >= (counter_t::max() / 2);
}

void BranchPredictorTypes::TwoLevelLocal::operator()(addr_t addr, bool taken, bool guess) {
	addr_t idx = mLHR[get_sbits<7>(addr)].value();
	if (taken) {
		++mPHT[idx];
	} else {
		--mPHT[idx];
	}
	mLHR[get_sbits<7>(addr)] << taken;
}