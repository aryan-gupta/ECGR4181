
#include <cmath>

#include "main.hpp"
#include "TwoLevelGSelect.hpp"

BranchPredictorTypes::TwoLevelGSelect::TwoLevelGSelect(unsigned shift, unsigned sat, unsigned addr)
	: mPHT{ (unsigned)std::pow(2, shift + addr), BitCounter{ sat } }
	, mGHT{ shift } {

}

bool BranchPredictorTypes::TwoLevelGSelect::operator()(addr_t addr) {
	addr_t idx = (get_sbits(addr, mAddrBits) << mAddrBits) +  mGHT.value();
	return mPHT[idx].value() >= (mPHT[idx].max() / 2);
}

void BranchPredictorTypes::TwoLevelGSelect::operator()(addr_t addr, bool taken, bool guess) {
	addr_t idx = (get_sbits(addr, mAddrBits) << mAddrBits) +  mGHT.value();
	if (taken) {
		++mPHT[idx];
	} else {
		--mPHT[idx];
	}
	mGHT << taken;
}