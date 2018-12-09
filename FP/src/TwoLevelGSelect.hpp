
#pragma once

#include <array>

#include "BranchPredictor.hpp"
#include "BitCounter.hpp"
#include "ShiftRegister.hpp"

namespace BranchPredictorTypes {

class TwoLevelGSelect : public BranchPredictor {
	std::vector<BitCounter> mPHT;
	ShiftRegister mGHT;
	unsigned mAddrBits;

public:
	TwoLevelGSelect(unsigned shift, unsigned sat, unsigned addr);

	virtual bool operator()(addr_t addr);
	virtual void operator()(addr_t addr, bool taken, bool guess);
};

}