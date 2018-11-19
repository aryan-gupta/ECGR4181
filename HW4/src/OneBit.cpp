

#include "main.hpp"
#include "OneBit.hpp"

bool BranchPredictorTypes::OneBit::operator()(addr_t addr) {
	return mPHT[get_sbits(addr)];
}

void BranchPredictorTypes::OneBit::operator()(addr_t addr, bool taken, bool guess) {
	mPHT[get_sbits(addr)] = taken;
}