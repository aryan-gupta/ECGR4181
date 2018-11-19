
#pragma once

#include <array>

#include "main.hpp"
#include "BranchPredictor.hpp"
#include "BitCounter.hpp"

namespace BranchPredictorTypes {

class Saturating : public BranchPredictor {
	using counter_t = BitCounter<2, uint8_t>;
	std::array<counter_t, 1024> mPHT;

public:
	virtual bool operator()(addr_t addr);
	virtual void operator()(addr_t addr, bool taken, bool);

};

}