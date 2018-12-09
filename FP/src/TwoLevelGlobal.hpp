
#include <vector>

#include "BranchPredictor.hpp"
#include "BitCounter.hpp"
#include "ShiftRegister.hpp"

namespace BranchPredictorTypes {

class TwoLevelGlobal : public BranchPredictor {
	std::vector<BitCounter> mPHT;
	ShiftRegister mGHT;

public:
	TwoLevelGlobal(unsigned sat_bits, unsigned sft_bits);

	virtual bool operator()(addr_t addr);
	virtual void operator()(addr_t addr, bool taken, bool guess);
};

}