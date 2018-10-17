
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

using ptr_t = uint32_t;
using access_type = std::vector<std::pair<Ops, ptr_t>>;

int main(int argn, char** args);

access_type load_file(static_string_t filename);

std::ostream& operator<< (std::ostream& out, Ops& op);
std::istream& operator>> (std::istream& in,  Ops& op);