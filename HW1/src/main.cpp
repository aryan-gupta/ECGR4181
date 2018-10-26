
#include <memory>
#include <cstring>
#include <iostream>
#include <fstream>
#include <filesystem>

#include "main.hpp"
#include "parse.hpp"
#include "Simulator.hpp"
#include "printer.hpp"

bool gUseSTDIN = false;

int main(int argn, char** args) {
	ParseData dat;
	try {
		dat = parse(args, argn);
	} catch (bad_prgm_argument& e) {
		std::cout << e.what() << std::endl;
		printer::help();
		return -1;
	}

	access_type opfile;
	if (dat.use_stdin) {
		opfile = load_stream(std::cin);
	} else {
		opfile = load_stream(std::ifstream{ std::string{ dat.file } });
	}

	if (opfile.size() == 0) {
		std::cerr << "[E] File " << dat.file << " not loaded, check if empty or not existant. ";
#ifdef __cpp_lib_filesystem
		std::cerr << "Current working directory is " << std::filesystem::current_path();
#endif
		std::cerr << std::endl;
		std::terminate();
	}

	printer::intro(dat);

	Simulator sim = Simulator{ std::move(opfile), dat };
	sim.simulate();

	printer::result(sim);
	return 0;
}

std::ostream& operator<< (std::ostream& out, Ops& op) {
	switch (op) {
		case Ops::READ: out << "READ"; break;
		case Ops::WRITE: out << "WRITE"; break;
		case Ops::FETCH: out << "FETCH"; break;
	}
	return out;
}

std::istream& operator>> (std::istream& in,  Ops& op) {
	std::string word;
	in >> word;

	char first = word[0];
	switch (first) {
		case 'R': op = Ops::READ; break;
		case 'W': op = Ops::WRITE; break;
		case 'F': op = Ops::FETCH; break;
		default: {
			int tmp = std::strtol(word.c_str(), nullptr, 10);
			if (errno == ERANGE) {
				in.setstate(std::ios::failbit);
			}
			op = static_cast<Ops>(tmp);
		}
	}
	return in;
}
