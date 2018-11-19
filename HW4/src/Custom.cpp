

#include "main.hpp"
#include "Custom.hpp"

bool BranchPredictorTypes::Custom::operator()(addr_t addr) {
	addr_t idx = get_sbits<10>(addr);
	return mLHR[idx][mGHT.value()].value() >= (counter_t::max() / 2);
}

void BranchPredictorTypes::Custom::operator()(addr_t addr, bool taken, bool guess) {
	addr_t idx = get_sbits<10>(addr);
	// mLHR[idx][mGHT.value()] = taken;
	if (taken) {
		++mLHR[idx][mGHT.value()];
	} else {
		--mLHR[idx][mGHT.value()];
	}
	mGHT << taken;
}