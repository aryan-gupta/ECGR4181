

#include <vector>
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <string>

#include "main.hpp"
#include "Simulator.hpp"

// These were orignially constexpr, however I modified it to not be
// will change case of lettering later
unsigned SIG_BITS;
unsigned LCO_BITS;

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

int main(int argn, char** argv) {
	ParseData dat;
	try {
		// parse through the command line arguments and return our arguments
		dat = parse(argv, argn); // Backwards I know
	} catch (bad_prgm_argument& e) {
		std::cout << e.what() << std::endl;
		print_help();
		return -1;
	}

	SIG_BITS = dat.significant_bits;
	LCO_BITS = dat.sig_lco_bits;

	std::cout << "Running branch predictor simulation using " << dat.predictor << " predictor with file: " << dat.file << std::endl;

	// Do we want to start parsing from a file or stdin (this allows us to combine `cat <file> | ./prgm.out [...]' and
	// `./prgm.out -f<file>')
	trace_t trace;
	if (false) {
		trace = load_stream(std::cin);
	} else {
		trace = load_stream(std::ifstream{ std::string{ "./project/branch-trace-gcc.trace" } }); // @TODO soft code this once we get parsing correct
	}

	// create our simulator using parsed trace file and our command line arguments
	Simulator sim{ std::move(trace), dat };
	sim(); // run the simulation

	// Print out the results
	std::cout << sim.getResults().first << " / " << sim.getResults().second << std::endl;
	std::cout << (double(sim.getResults().first) / sim.getResults().second) << std::endl;

	return 0;
}

void print_help() {
	std::cout << "There is no help, you are alone...." << std::endl; // will work on this later
}

std::ostream& operator<< (std::ostream& out, Predictor op) {
	// Convert an enum to a text object
	switch (op) {
		case Predictor::ALWAYST: out << "Always Taken"; break;
		case Predictor::ALWAYSN: out << "Always Not Taken"; break;
		case Predictor::ONE_BIT: out << "One Bit"; break;
		case Predictor::TWO_BIT: out << "Two Bit"; break;
		case Predictor::GLOBAL: out << "Global"; break;
		case Predictor::GSHARE: out << "GShare"; break;
		case Predictor::GSELECT: out << "GSelect"; break;
		case Predictor::LOCAL: out << "Local"; break;
		// So vscode is being an idiot and marking this as an error. I am severly bothered by the red swigly line
		// Im going to have to go the long route
		default: throw std::invalid_argument{ "[E] " + std::to_string((int)op) + " is not a valid predictor" };
	}
	return out;
}

// returns the significant bits of an address
addr_t get_sbits(addr_t addr, unsigned sig, unsigned lco) {
	// get number of bits in addr_t and subtract 10, this will give us the amount to shift
	// right to get a mask.
	addr_t mask = std::numeric_limits<addr_t>::max() >> ((sizeof(addr_t) * 8) - sig);
	addr >>= lco;
	return addr bitand mask;
}