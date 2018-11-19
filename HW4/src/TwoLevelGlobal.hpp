
#include <array>

#include "BranchPredictor.hpp"
#include "BitCounter.hpp"
#include "ShiftRegister.hpp"

namespace BranchPredictorTypes {

class TwoLevelGlobal : public BranchPredictor {
	using counter_t = BitCounter<2, uint8_t>;
	std::array<counter_t, 1024> mPHT;
	ShiftRegister<10, uint16_t> mGHT;

public:
	virtual bool operator()(addr_t addr);
	virtual void operator()(addr_t addr, bool taken, bool guess);
};

}