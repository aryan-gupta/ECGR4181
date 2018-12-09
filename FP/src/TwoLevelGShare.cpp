
#include <cmath>

#include "main.hpp"
#include "TwoLevelGShare.hpp"

BranchPredictorTypes::TwoLevelGShare::TwoLevelGShare(unsigned shift, unsigned sat)
	: mPHT( (unsigned)std::pow(2, shift), BitCounter{ sat } )
	, mGHT{ shift }
	{  }

bool BranchPredictorTypes::TwoLevelGShare::operator()(addr_t addr) {
	addr_t idx = get_sbits(addr) ^ mGHT.value();
	return mPHT[idx].value() >= (mPHT[idx].max() / 2);
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