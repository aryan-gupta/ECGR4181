

#include "main.hpp"
#include "Custom.hpp"

bool BranchPredictorTypes::Custom::operator()(addr_t addr) {
	addr_t idx = get_sbits<SIG_BITS>(addr);
	return mLHR.at(idx).at(mGHT.value()).value() >= (counter_t::max() / 2);
}

void BranchPredictorTypes::Custom::operator()(addr_t addr, bool taken, bool) {
	addr_t idx = get_sbits<SIG_BITS>(addr);
	// mLHR[idx][mGHT.value()] = taken;
	if (taken) {
		++mLHR.at(idx).at(mGHT.value());
	} else {
		--mLHR.at(idx).at(mGHT.value());
	}
	mGHT << taken;
}