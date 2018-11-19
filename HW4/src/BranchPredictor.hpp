
#pragma once

#include <limits>

#include "main.hpp"

enum class Predictor {
	ONE_LVL,
	TWO_LVL_GLOBAL,
	TWO_LVL_GSHARE,
	TWO_LVL_LOCAL
};

namespace BranchPredictorTypes {
// An interface for all Branch predictors. I did somthing similar to this
// in HW1 using functions that return lambdas, and found that its alot easier
// if I do this. Though the polymorphism might slow down the code, its alot
// easier to understand

class BranchPredictor {
protected:
	// returns the significant bits of an address
	static constexpr addr_t get_sbits(addr_t addr) {
		// get number of bits in addr_t and subtract 10, this will give us the amount to shift
		// right to get a mask.
		constexpr addr_t mask = std::numeric_limits<addr_t>::max() >> ((sizeof(addr_t) * 8) - 10);
		return addr bitand mask;
	}

public:
	// So we can do delete from a base class
	virtual ~BranchPredictor() = default;

	// This operator returns if we should take the branch or not take it. I takes in an
	// address and uses whatever meathod to determine its prediction
	virtual bool operator()(addr_t a) = 0;

	// This operator updates its database depending on the actual.
	// @param a The address of the branch
	// @param b Whether the branch was taken or not
	// @param c Our prediction from above operator. This is to prevent recalculation.
	virtual void operator()(addr_t a, bool b, bool c) = 0;
};

}