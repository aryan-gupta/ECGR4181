

#include <vector>
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <string>

#include "main.hpp"
#include "Simulator.hpp"

// These were orignially constexpr, however I modified it to not be
// will change case of lettering later
unsigned SIG_BITS;
unsigned LCO_BITS;

std::vector<std::string> split_line(const std::string& str, char delim = ' ') {
	std::vector<std::string> ret;
	std::stringstream ss(str);
	std::string token;
	while (std::getline(ss, token, delim))
		ret.push_back(token);
	return ret;
}

template <typename T, typename = std::enable_if_t<std::is_base_of<std::istream, typename std::decay_t<T>>::value>>
trace_t load_stream(T&& stream) {
	if (!stream) throw std::invalid_argument{ "[E] stream cannot be empty" };

	trace_t ret_val;

	for (std::string line; std::getline(stream, line); ) {
		auto split = split_line(line);

		addr_t addr = std::stoll(split[0], nullptr, 16);
		bool taken = (split[1] == "T")? true : false;
		addr_t target = std::stoll(split[2], nullptr, 16);
		bool bias = split[3] == "0";

		ret_val.push_back({ addr, taken, target, { bias } } );
	}

	return ret_val;
}

int main(int argn, char** argv) {
	ParseData dat {
		.file = "project/branch-trace-gcc.trace",
		.predictor = Predictor::ALWAYST,
		.agree_base = Predictor::ALWAYST,
		.saturation_bits = 2,
		.significant_bits = 10,
		.sig_lco_bits = 0,
		.shift_reg_bits = 10,
		.lhr_bits = 7,
		.addr_bits = 5,

		.use_stdin = false
	};

	try {
		// parse through the command line arguments and return our arguments
		parse(dat, argn, argv);
	} catch (bad_prgm_argument& e) {
		std::cout << e.what() << std::endl;
		print_help();
		return -1;
	}

	SIG_BITS = dat.significant_bits;
	LCO_BITS = dat.sig_lco_bits;

	std::cout << "Running branch predictor simulation using " << dat.predictor << " predictor with file: " << dat.file << std::endl;
	std::cout << ".file = " << dat.file << std::endl;
	std::cout << ".predictor = " << dat.predictor << std::endl;
	std::cout << ".agree_base = " << dat.agree_base << std::endl;
	std::cout << ".saturation_bits = " << dat.saturation_bits << std::endl;
	std::cout << ".significant_bits = " << dat.significant_bits << std::endl;
	std::cout << ".sig_lco_bits = " << dat.sig_lco_bits << std::endl;
	std::cout << ".shift_reg_bits = " << dat.shift_reg_bits << std::endl;
	std::cout << ".lhr_bits = " << dat.lhr_bits << std::endl;
	std::cout << ".addr_bits = " << dat.addr_bits << std::endl;
	std::cout << ".use_stdin = " << std::boolalpha << dat.use_stdin << std::noboolalpha << std::endl;

	// Do we want to start parsing from a file or stdin (this allows us to combine `cat <file> | ./prgm.out [...]' and
	// `./prgm.out -f<file>')
	trace_t trace;
	if (dat.use_stdin) {
		trace = load_stream(std::cin);
	} else {
		trace = load_stream(std::ifstream{ std::string{ dat.file } }); // @TODO soft code this once we get parsing correct
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
		case Predictor::ALWAYST:    out << "Always Taken"; break;
		case Predictor::ALWAYSN:    out << "Always Not Taken"; break;
		case Predictor::ONE_BIT:    out << "One Bit"; break;
		case Predictor::TWO_BIT:    out << "Two Bit"; break;
		case Predictor::GLOBAL:     out << "Global"; break;
		case Predictor::GSHARE:     out << "GShare"; break;
		case Predictor::GSELECT:    out << "GSelect"; break;
		case Predictor::LOCAL:      out << "Local"; break;
		case Predictor::BTFN:       out << "Backwards Taken Forwards Not Taken"; break;
		case Predictor::AGREEBTFN:  out << "Agree Backwards Biased Forwards Not Biased"; break;
		case Predictor::AGREEFIRST: out << "Agree Use First branch prediction"; break;
		case Predictor::AGREEISA:   out << "Agree Use ISA Extension"; break;
		// So vscode is being an idiot and marking this as an error. I am severly bothered by the red swigly line
		// Im going to have to go the long route
		default: throw std::invalid_argument{ "[E] " + std::to_string((int)op) + " is not a valid predictor" };
	}
	return out;
}

// returns the significant bits of an address
addr_t get_sbits(addr_t addr, unsigned sig, unsigned lco) {
	if (sig == 65) sig = SIG_BITS;
	if (lco == 65) lco = LCO_BITS;
	// get number of bits in addr_t and subtract 10, this will give us the amount to shift
	// right to get a mask.
	addr_t mask = std::numeric_limits<addr_t>::max() >> ((sizeof(addr_t) * 8) - sig);
	addr >>= lco;
	return addr bitand mask;
}