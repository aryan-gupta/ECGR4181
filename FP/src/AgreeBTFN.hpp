
#pragma once

#include <unordered_map>

#include "BranchPredictor.hpp"

// The essence of the AgreeBTFN predictor is that it uses the mPHT's

namespace BranchPredictorTypes {

template <typename T>
class AgreeBTFN : public T {
	bool mBias;

public:
	template <typename... P>
	AgreeBTFN(P&&... args)
		: T{ std::forward<P>(args)... } {  }

	virtual bool operator()(addr_t addr, addr_t target, Extensions) {
		bool mBias = target > addr;
		bool taken = T::operator()(addr, target, Extensions{  }); // call super's guess
		// if our base predictor agree's with our bias
		return taken == mBias;
	}

	virtual void operator()(addr_t addr, bool taken, bool) {
		T::operator()(addr, taken == mBias, true);
	}

};

}