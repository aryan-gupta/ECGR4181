

#include "main.hpp"
#include "Always.hpp"

BranchPredictorTypes::Always::Always(bool taken) : mTaken{ taken } {}

bool BranchPredictorTypes::Always::operator()(addr_t addr) {
	return mTaken;
}

void BranchPredictorTypes::Always::operator()(addr_t addr, bool taken, bool guess) {
	return;
}