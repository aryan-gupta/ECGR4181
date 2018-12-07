

#include "main.hpp"
#include "TwoLevelGSelect.hpp"

bool BranchPredictorTypes::TwoLevelGSelect::operator()(addr_t addr) {
	addr_t idx = (get_sbits<ADDR_BITS>(addr) << ADDR_BITS) +  mGHT.value();
	return mPHT[idx].value() >= (counter_t::max() / 2);
}

void BranchPredictorTypes::TwoLevelGSelect::operator()(addr_t addr, bool taken, bool guess) {
	addr_t idx = (get_sbits<ADDR_BITS>(addr) << ADDR_BITS) + mGHT.value();
	if (taken) {
		++mPHT[idx];
	} else {
		--mPHT[idx];
	}
	mGHT << taken;
}