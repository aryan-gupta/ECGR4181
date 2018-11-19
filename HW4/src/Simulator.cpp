

#include "main.hpp"
#include "Simulator.hpp"
#include "Saturating.hpp"

Simulator::Simulator(trace_t&& trace, ParseData dat)
	: mTrace{ std::move(trace) }
	, mBP{ new BranchPredictorTypes::Saturating() }
{

}

Simulator::~Simulator() {
	delete mBP;
}

void Simulator::operator()() {
	BranchPredictorTypes::BranchPredictor& bp = *mBP;

	for (auto& p : mTrace) {
		++mTotal;

		bool guess = bp(p.first);
		bp(p.first, p.second, guess);

		if (guess == p.second) {
			++mCorrect;
		}
	}
}

ratio_t Simulator::getResults() {
	return { mCorrect, mTotal };
}