

#include <vector>
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <string>

using addr_t = uint64_t; // type for storing addresses
using trace_t = std::vector<std::pair<addr_t, bool>>;

template <typename T, typename = std::enable_if_t<std::is_base_of<std::istream, typename std::decay_t<T>>::value>>
trace_t load_stream(T&& stream) {
	if (!stream) throw std::invalid_argument{ "[E] stream cannot be empty" };

	trace_t ret_val;

	for (std::string line; std::getline(stream, line); ) {
		addr_t addr = std::atoi(line.data());
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
		trace = load_stream(std::ifstream{ std::string{ "./project/branch-trace-gcc.trace" } });
	}

	for (int i = 0; i < 10; ++i) {
		std::cout << trace[i].first << " " << trace[i].second << std::endl;
	}

	return 0;
}