

#include <vector>
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <string>

#include "main.hpp"
#include "Simulator.hpp"

int main(int argn, char** argv) {
	ParseData dat;
	try {
		dat = parse(argv, argn); // Backwards I know
	} catch (bad_prgm_argument& e) {
		std::cout << e.what() << std::endl;
		print_help();
		return -1;
	}

	std::cout << "Running branch predictor simulation using " << dat.predictor << " predictor with file: " << dat.file << std::endl;

	trace_t trace;
	if (false) {
		trace = load_stream(std::cin);
	} else {
		trace = load_stream(std::ifstream{ std::string{ "./project/branch-trace-gcc.trace" } }); // @TODO soft code this once we get parsing correct
	}

	Simulator sim{ std::move(trace), dat };
	sim();
	std::cout << sim.getResults().first << " / " << sim.getResults().second << std::endl;
	std::cout << (double(sim.getResults().first) / sim.getResults().second) << std::endl;

	return 0;
}

void print_help() {
	std::cout << "There is no help, you are alone...." << std::endl; // will work on this later
}

std::ostream& operator<< (std::ostream& out, Predictor op) {
	switch (op) {
		case Predictor::ALWAYST: out << "Always Taken"; break;
		case Predictor::ALWAYSN: out << "Always Not Taken"; break;
		case Predictor::ONE_BIT: out << "One Bit"; break;
		case Predictor::TWO_BIT: out << "Two Bit"; break;
		case Predictor::GLOBAL: out << "Global"; break;
		case Predictor::GSHARE: out << "GShare"; break;
		case Predictor::GSELECT: out << "GSelect"; break;
		case Predictor::LOCAL: out << "Local"; break;
		case Predictor::CUSTOM: out << "Custom"; break;
		// So vscode is being an idiot and marking this as an error. I am severly bothered by the red swigly line
		// Im going to have to go the long route
		default: throw std::invalid_argument{ "[E] " + std::to_string((int)op) + " is not a valid predictor" };
	}
	return out;
}