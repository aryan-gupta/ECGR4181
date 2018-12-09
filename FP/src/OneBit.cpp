
#include <cmath>

#include "main.hpp"
#include "OneBit.hpp"

BranchPredictorTypes::OneBit::OneBit(unsigned pht_bits)
	: mPHT( static_cast<unsigned>(std::pow(2, pht_bits)) ) {  }

bool BranchPredictorTypes::OneBit::operator()(addr_t addr) {
	return mPHT[get_sbits(addr)];
}

void BranchPredictorTypes::OneBit::operator()(addr_t addr, bool taken, bool) {
	mPHT[get_sbits(addr)] = taken;
}