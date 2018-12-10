
#pragma once

#include <limits>

#include "main.hpp"


namespace BranchPredictorTypes {
// An interface for all Branch predictors. I did somthing similar to this
// in HW1 using functions that return lambdas, and found that its alot easier
// if I do this. Though the polymorphism might slow down the code, its alot
// easier to understand

struct BranchPredictor {
	// So we can do delete from a base class
	virtual ~BranchPredictor() = default;

	// This operator returns if we should take the branch or not take it. I takes in an
	// address and uses whatever meathod to determine its prediction
	// The b object is the target of the branch
	virtual bool operator()(addr_t a, addr_t b, Extensions c) = 0;

	// This operator updates its database depending on the actual.
	// @param a The address of the branch
	// @param b Whether the branch was taken or not
	// @param c Our prediction from above operator. This is to prevent recalculation.
	virtual void operator()(addr_t a, bool b, bool c) = 0;
};

}