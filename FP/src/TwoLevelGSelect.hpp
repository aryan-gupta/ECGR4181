
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
	TwoLevelGSelect(unsigned pht, unsigned sat, unsigned addr);

	virtual bool operator()(addr_t addr, addr_t, Extensions);
	virtual void operator()(addr_t addr, bool taken, bool guess);
};

}