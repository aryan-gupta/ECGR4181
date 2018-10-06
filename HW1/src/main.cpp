
#include <memory>
#include <cstring>
#include <iostream>
#include <ifstream>

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

	load_file(dat.file);

	std::cout << dat.cache_size << std::endl;
	std::cout << dat.block_size << std::endl;
	std::cout << dat.use_stdin << std::endl;
	std::cout << dat.replace_policy << std::endl;
	std::cout << (int)dat.associativity << std::endl;
}


std::vector<std::pair<uint8_t, uint64_t>> load_file(static_string_t filename) {
	std::ifstream file{ filename };
	for (
		auto [idx, line] = std::pair{ size_t{}, std::string{} };
		std::getline(file, line);
	) {

	}
}