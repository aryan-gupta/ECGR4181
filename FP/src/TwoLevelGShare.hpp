
#pragma once

#include <array>

#include "BranchPredictor.hpp"
#include "BitCounter.hpp"
#include "ShiftRegister.hpp"

namespace BranchPredictorTypes {

class TwoLevelGShare : public BranchPredictor {
	using counter_t = BitCounter<BIT_CNT>;
	std::array<counter_t, SFT_BITS> mPHT;
	ShiftRegister<SFT_BITS> mGHT;

public:
	virtual bool operator()(addr_t addr);
	virtual void operator()(addr_t addr, bool taken, bool guess);
};

}