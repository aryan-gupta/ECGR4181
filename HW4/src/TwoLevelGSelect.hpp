
#pragma once

#include <array>

#include "BranchPredictor.hpp"
#include "BitCounter.hpp"
#include "ShiftRegister.hpp"

namespace BranchPredictorTypes {

class TwoLevelGSelect : public BranchPredictor {
	using counter_t = BitCounter<2>;

	static constexpr unsigned ADDR_BITS = 5;

	std::array<counter_t, 1024> mPHT;
	ShiftRegister<5> mGHT;

public:
	virtual bool operator()(addr_t addr);
	virtual void operator()(addr_t addr, bool taken, bool guess);
};

}