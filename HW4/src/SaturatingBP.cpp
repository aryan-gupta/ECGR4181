
#include "SaturatingBP.hpp"

bool BranchPredictorTypes::SaturatingBP::operator()(addr_t addr) {
	addr_t idx = get_sbits(addr);

	if (mPHT[idx].value() >= (counter_t::max() / 2)) {
		// std::cout <<
		return true;
	}
	return false;
}

void BranchPredictorTypes::SaturatingBP::operator()(addr_t addr, bool taken) {
	if (taken) {
		++mPHT[get_sbits(addr)];
	} else {
		--mPHT[get_sbits(addr)];
	}
}