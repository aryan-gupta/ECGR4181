
#include <cmath>

#include "main.hpp"
#include "TwoLevelGlobal.hpp"


BranchPredictorTypes::TwoLevelGlobal::TwoLevelGlobal(unsigned sft_bits, unsigned sat_bits)
	: mPHT( (unsigned)std::pow(2, sft_bits), BitCounter{ sat_bits } )
	, mGHT{ sft_bits } {  }

bool BranchPredictorTypes::TwoLevelGlobal::operator()(addr_t addr, addr_t, Extensions) {
	addr_t idx = mGHT.value();
	return mPHT[idx].value() >= (mPHT[idx].max() / 2);
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