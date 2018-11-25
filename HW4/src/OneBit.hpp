
#pragma once

#include <array>
#include <bitset>

#include "BranchPredictor.hpp"

namespace BranchPredictorTypes {
// for each branch address, there is a 1024 bit array. The BP looks up the address and gets an array. 
// Then the BP uses the GHR to find the index, which stores whether the branch should be taken or not
// So essentially a Local History Table coupled with 

// I dont like the naming scheme, too generic
class OneBit : public BranchPredictor {
	std::bitset<pow(2, SIG_BITS)> mPHT;

public:
	virtual bool operator()(addr_t addr);
	virtual void operator()(addr_t addr, bool taken, bool);
};

}