
#pragma once

#include <array>

#include "BranchPredictor.hpp"
#include "BitCounter.hpp"
#include "ShiftRegister.hpp"

namespace BranchPredictorTypes {

class TwoLevelLocal : public BranchPredictor {
	using counter_t = BitCounter<BIT_CNT>;
	using register_t = ShiftRegister<10>;

	static constexpr unsigned LHR_BITS = 7;

	std::array<counter_t, pow(2, SIG_BITS)> mPHT;
	std::array<register_t, pow(2, LHR_BITS)> mLHR;

public:
	virtual bool operator()(addr_t addr);
	virtual void operator()(addr_t addr, bool taken, bool guess);
};

}