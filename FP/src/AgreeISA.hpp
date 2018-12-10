
#include <unordered_map>

#include "BranchPredictor.hpp"

namespace BranchPredictorTypes {

template <typename T>
class AgreeISA : public T {
	bool mBias;

public:
	template <typename... P>
	AgreeISA(P&&... args)
		: T{ std::forward<P>(args)... } {  }

	virtual bool operator()(addr_t addr, addr_t target, Extensions c) {
		bool mBias = c.bias;
		bool taken = T::operator()(addr, target, c); // call super's guess
		// if our base predictor agree's with our bias
		return taken == mBias;
	}

	virtual void operator()(addr_t addr, bool taken, bool) {
		T::operator() (addr, taken == mBias, true);
	}

};

}
