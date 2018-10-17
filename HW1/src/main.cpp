
#include <memory>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>

#include "main.hpp"
#include "parse.hpp"
#include "Simulator.hpp"

bool gUseSTDIN = false;

int main(int argn, char** args) {
	ParseData dat;
	try {
		dat = parse(args, argn);
	} catch (bad_prgm_argument& e) {
		std::cout << e.what() << std::endl;
		print_help(std::cout);
		return -1;
	}

	dat.associativity = 1;
	dat.cache_size = 128;
	dat.block_size = 4;

	access_type opfile = load_file(dat.file);

	for (auto& p : opfile) {
		std::cout << p.first << "  " << p.second << std::endl;
	}

	/// from ping(8):
	// For every ECHO_REQUEST sent a period “.” is printed, while for ever ECHO_REPLY received a backspace is
	// printed. This provides a rapid display of how many packets are being dropped.
	// Also just realized they misspelled every
	// I fell like there will be a point where the period is spammed so I may represent = by 10 periods? idk
	std::cout << "File and parameters successfully loaded." "\n"
	             "For every memory access a period \".\" is printed, while for every memory hit a backspace is"
	             "printed, this provides a rapid display of how many cache misses there are" "\n";

	Simulator sim = Simulator{ std::move(opfile), dat };
	sim.doSim();

	// for (auto& p : opfile) {
	// 	std::cout << p.first << "  " << p.second << std::endl;
	// }

	// std::cout << dat.cache_size << std::endl;
	// std::cout << dat.block_size << std::endl;
	// std::cout << dat.use_stdin << std::endl;
	// std::cout << dat.replace_policy << std::endl;
	// std::cout << (int)dat.associativity << std::endl;
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
