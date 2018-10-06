
#pragma once

#include <memory>
#include <string>
#include <vector>
#include <utility>
#include <iostream>

#include "parse.hpp"

extern bool gUseSTDIN;

enum class Ops {
	READ,
	WRITE,
	FETCH
};

int main(int argn, char** args);

std::vector<std::pair<Ops, uint64_t>> load_file(static_string_t filename);

std::ostream& operator<< (std::ostream& out, Ops& op);
std::istream& operator>> (std::istream& in,  Ops& op);