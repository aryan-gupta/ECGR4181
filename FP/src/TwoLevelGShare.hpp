
#pragma once

#include <vector>

#include "BranchPredictor.hpp"
#include "BitCounter.hpp"
#include "ShiftRegister.hpp"

namespace BranchPredictorTypes {

class TwoLevelGShare : public BranchPredictor {
	std::vector<BitCounter> mPHT;
	ShiftRegister mGHT;

public:
	TwoLevelGShare(unsigned shift, unsigned sat);

	virtual bool operator()(addr_t addr, addr_t, Extensions);
	virtual void operator()(addr_t addr, bool taken, bool guess);
};

}