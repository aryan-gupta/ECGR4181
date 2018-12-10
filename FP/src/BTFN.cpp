
#include <cmath>

#include "main.hpp"
#include "BTFN.hpp"

bool BranchPredictorTypes::BTFN::operator()(addr_t addr, addr_t target, Extensions) {
	return target < addr;
}

void BranchPredictorTypes::BTFN::operator()(addr_t addr, bool taken, bool) {
	return;
}