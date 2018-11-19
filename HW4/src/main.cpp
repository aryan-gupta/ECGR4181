

#include <vector>
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <string>

#include "main.hpp"
#include "Simulator.hpp"

template <typename T, typename = std::enable_if_t<std::is_base_of<std::istream, typename std::decay_t<T>>::value>>
trace_t load_stream(T&& stream) {
	if (!stream) throw std::invalid_argument{ "[E] stream cannot be empty" };

	trace_t ret_val;

	for (std::string line; std::getline(stream, line); ) {
		addr_t addr = std::stoll(line);
		bool taken = (line[line.size() - 1] == 'T')? true : false;

		ret_val.push_back({ addr, taken });
	}

	return ret_val;
}

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