

#include "main.hpp"
#include "Always.hpp"

BranchPredictorTypes::Always::Always(bool taken) : mTaken{ taken } {}

bool BranchPredictorTypes::Always::operator()(addr_t) {
	return mTaken;
}

void BranchPredictorTypes::Always::operator()(addr_t, bool, bool) {
	return;
}