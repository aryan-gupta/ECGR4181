
#pragma once

#include <vector>

#include "BranchPredictor.hpp"
#include "BitCounter.hpp"
#include "ShiftRegister.hpp"

namespace BranchPredictorTypes {

class TwoLevelLocal : public BranchPredictor {
	std::vector<BitCounter> mPHT;
	std::vector<ShiftRegister> mLHR;
	unsigned mLHRBits;

public:
	TwoLevelLocal(unsigned sft, unsigned sat, unsigned lhr);

	virtual bool operator()(addr_t addr, addr_t, Extensions);
	virtual void operator()(addr_t addr, bool taken, bool guess);
};

}