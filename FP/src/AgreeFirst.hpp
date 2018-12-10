
#include <unordered_map>

#include "BranchPredictor.hpp"

// The essence of the AgreeFirst predictor is that it uses the mPHT's

namespace BranchPredictorTypes {

template <typename T>
class AgreeFirst : public T {
	std::unordered_map<addr_t, bool> mBHT;

public:
	template <typename... P>
	AgreeFirst(P&&... args)
		: T{ std::forward<P>(args)... } {  }

	virtual bool operator()(addr_t addr, addr_t target, Extensions) {
		if (mBHT.find(addr) == mBHT.end()) {
			mBHT[addr] = target < addr;
		}

		bool taken = T::operator()(addr, target, Extensions{  }); // call super's guess
		// if our base predictor agree's with our bias
		return taken == mBHT[addr];
	}

	virtual void operator()(addr_t addr, bool taken, bool) {
		T::operator()(addr, taken == mBHT[addr], true);
	}

};

}