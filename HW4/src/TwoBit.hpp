
#pragma once

#include <array>

#include "main.hpp"
#include "BranchPredictor.hpp"
#include "BitCounter.hpp"

namespace BranchPredictorTypes {

class TwoBit : public BranchPredictor {
	using counter_t = BitCounter<BIT_CNT>;
	std::array<counter_t, pow(2, SIG_BITS)> mPHT;

public:
	virtual bool operator()(addr_t addr);
	virtual void operator()(addr_t addr, bool taken, bool);

};

}