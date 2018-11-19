

#include <vector>
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <string>

#include "main.hpp"
#include "Simulator.hpp"

int main() {

	trace_t trace;
	if (false) {
		trace = load_stream(std::cin);
	} else {
		trace = load_stream(std::ifstream{ std::string{ "./project/branch-trace-gcc.trace" } }); // @TODO soft code this once we get parsing correct
	}

	Simulator sim{ std::move(trace), {} };
	sim();
	std::cout << sim.getResults().first << " / " << sim.getResults().second << std::endl;
	std::cout << (double(sim.getResults().first) / sim.getResults().second) << std::endl;

	return 0;
}