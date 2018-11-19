
#pragma once

#include "main.hpp"
#include "parse.hpp"
#include "BranchPredictor.hpp"

// a predictor type is a callable class that takes in

class Simulator {
	trace_t mTrace;
	BranchPredictorTypes::BranchPredictor* mBP;
	uint64_t mCorrect;
	uint64_t mTotal;

public:
	Simulator() = delete;
	Simulator(trace_t&& trace, ParseData dat);
	~Simulator();

	void operator()();

	ratio_t getResults();

};