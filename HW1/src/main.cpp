
#include <memory>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>

#include "main.hpp"
#include "parse.hpp"

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

	auto opfile = load_file(dat.file);

	for (auto& p : opfile) {
		std::cout << p.first << "  " << p.second << std::endl;
	}

	std::cout << dat.cache_size << std::endl;
	std::cout << dat.block_size << std::endl;
	std::cout << dat.use_stdin << std::endl;
	std::cout << dat.replace_policy << std::endl;
	std::cout << (int)dat.associativity << std::endl;
}

std::vector<std::pair<Ops, uint64_t>> load_file(static_string_t filename) {
	std::vector<std::pair<Ops, uint64_t>> ret;

	std::ifstream file{ std::string { filename } };
	for (std::string line{  }; std::getline(file, line); ) {
		std::stringstream ss{ line };
		Ops code;
		uint64_t location;

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