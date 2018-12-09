
#include <cmath>

#include "main.hpp"
#include "TwoLevelLocal.hpp"

BranchPredictorTypes::TwoLevelLocal::TwoLevelLocal(unsigned sft, unsigned sat, unsigned lhr)
	: mPHT{ (unsigned)std::pow(2, sft), BitCounter{ sat } }
	, mLHR{ (unsigned)std::pow(2, lhr), ShiftRegister{ sft } }
	{  }

bool BranchPredictorTypes::TwoLevelLocal::operator()(addr_t addr) {
	addr_t idx = mLHR[get_sbits(addr, mLHRBits)].value();
	return mPHT[idx].value() >= (mPHT[idx].max() / 2);
}

void BranchPredictorTypes::TwoLevelLocal::operator()(addr_t addr, bool taken, bool guess) {
	addr_t idx = mLHR[get_sbits(addr, mLHRBits)].value();
	if (taken) {
		++mPHT[idx];
	} else {
		--mPHT[idx];
	}
	mLHR[idx] << taken;
}