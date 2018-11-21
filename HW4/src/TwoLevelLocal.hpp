
#pragma once

#include <array>

#include "BranchPredictor.hpp"
#include "BitCounter.hpp"
#include "ShiftRegister.hpp"

namespace BranchPredictorTypes {

class TwoLevelLocal : public BranchPredictor {
	using counter_t = BitCounter<gBitCounterSize>;
	using register_t = ShiftRegister<10>;

	std::array<counter_t, 1024> mPHT;
	std::array<register_t, 128> mLHR;

public:
	virtual bool operator()(addr_t addr);
	virtual void operator()(addr_t addr, bool taken, bool guess);
};

}