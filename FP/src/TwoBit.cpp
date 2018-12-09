
#include <cmath>

#include "TwoBit.hpp"

BranchPredictorTypes::TwoBit::TwoBit(unsigned pht_bits, unsigned sat_bits)
	: mPHT{ (unsigned)std::pow(2, pht_bits), BitCounter{ sat_bits } }
	{  }

bool BranchPredictorTypes::TwoBit::operator()(addr_t addr) {
	addr_t idx = get_sbits(addr);
	return mPHT[idx].value() >= (mPHT[idx].max() / 2);
}

void BranchPredictorTypes::TwoBit::operator()(addr_t addr, bool taken, bool) {
	if (taken) {
		++mPHT[get_sbits(addr)];
	} else {
		--mPHT[get_sbits(addr)];
	}
}