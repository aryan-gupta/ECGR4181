
#include <memory>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
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

	access_type opfile = load_file(dat.file);
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

access_type load_file(static_string_t filename) {
	std::vector<std::pair<Ops, ptr_t>> ret;

	std::ifstream file{ std::string { filename } };
	for (std::string line{  }; std::getline(file, line); ) {
		std::stringstream ss{ line };
		Ops code;
		ptr_t location;

		ss >> code >> std::hex >> location >> std::dec;

		ret.emplace_back(code, location);
	}

	return ret;
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
