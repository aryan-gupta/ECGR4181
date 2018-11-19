
#pragma once

#include <limits>

#include "main.hpp"

// Interface for virual functions.
// The only requirement of a branch predictor is that it
// needs a virtual `bool operator(addr_t)` that returns if
// the branch should be taken or not. And another virtual
// `bool operator(addr_t, bool)` if branch was taken or not.

namespace BranchPredictorTypes {

class BranchPredictor {
	// returns the significant bits of addr
protected:
	static constexpr addr_t get_sbits(addr_t addr) {
		// get number of bits in addr_t and subtract 10, this will give us the amount to shift
		// right to get a mask.
		constexpr addr_t mask = std::numeric_limits<addr_t>::max() >> ((sizeof(addr_t) * 8) - 10);
		return addr bitand mask;
	}

public:
	virtual bool operator()(addr_t) = 0;
	virtual void operator()(addr_t, bool) = 0;
};

}