
#pragma once

#include "BranchPredictor.hpp"

namespace BranchPredictorTypes {

class BTFN : public BranchPredictor {

public:
	virtual bool operator()(addr_t addr, addr_t target, Extensions);
	virtual void operator()(addr_t addr, bool taken, bool);
};

}