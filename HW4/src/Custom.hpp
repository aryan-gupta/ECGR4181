
#pragma once

#include <array>
#include <bitset>

#include "BranchPredictor.hpp"
#include "BitCounter.hpp"
#include "ShiftRegister.hpp"

namespace BranchPredictorTypes {
// for each branch address, there is a 1024 bit array. The BP looks up the address and gets an array. 
// Then the BP uses the GHR to find the index, which stores whether the branch should be taken or not
// So essentially a Local History Table coupled with 

// I dont like the naming scheme, too generic
class Custom : public BranchPredictor {
	using counter_t = BitCounter<2>;
	using register_t = ShiftRegister<10>;
	//using bitarray_t = std::bitset<1024>;
	using bitarray_t = std::array<counter_t, 1024>;
	
	register_t mGHT;
	std::array<bitarray_t, 1024> mLHR;

public:
	virtual bool operator()(addr_t addr);
	virtual void operator()(addr_t addr, bool taken, bool guess);
};

}