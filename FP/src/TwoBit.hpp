
#pragma once

#include <vector>

#include "main.hpp"
#include "BranchPredictor.hpp"
#include "BitCounter.hpp"

namespace BranchPredictorTypes {

class TwoBit : public BranchPredictor {
	std::vector<BitCounter> mPHT;

public:
	TwoBit(unsigned pht_bits, unsigned sat_bits);

	virtual bool operator()(addr_t addr, addr_t, Extensions);
	virtual void operator()(addr_t addr, bool taken, bool);

};

}