
#pragma once

#include <memory>
#include <string>
#include <vector>
#include <utility>

#include "parse.hpp"

extern bool gUseSTDIN;

int main(int argn, char** args);

std::vector<std::pair<uint8_t, uint64_t>> load_file(static_string_t filename);