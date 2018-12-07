
#pragma once

#include <array>

#include "BranchPredictor.hpp"
#include "BitCounter.hpp"
#include "ShiftRegister.hpp"

namespace BranchPredictorTypes {

class TwoLevelGSelect : public BranchPredictor {
	using counter_t = BitCounter<BIT_CNT>;

	static constexpr unsigned ADDR_BITS = 5;
	static constexpr unsigned SHIFT_BITS = SFT_BITS - ADDR_BITS;

	std::array<counter_t, pow(2, ADDR_BITS + SHIFT_BITS)> mPHT;
	ShiftRegister<SHIFT_BITS> mGHT;

public:
	virtual bool operator()(addr_t addr);
	virtual void operator()(addr_t addr, bool taken, bool guess);
};

}