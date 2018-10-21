
#pragma once

#include <memory>
#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <type_traits>

#include "parse.hpp"

/// Stolen from https://stackoverflow.com/questions/3767869/adding-message-to-assert
#ifndef NDEBUG
#   define ASSERT(condition, message) \
	do { \
		if (! (condition)) { \
			std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
					  << " line " << __LINE__ << ": " << message << std::endl; \
			std::terminate(); \
		} \
	} while (false)
#else
#   define ASSERT(condition, message) do { } while (false)
#endif

extern bool gUseSTDIN;

enum class Ops {
	READ,
	WRITE,
	FETCH
};

using ptr_t = uint32_t;
using access_type = std::vector<std::pair<Ops, ptr_t>>;

template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>> // std::is_integral_v<T> is c++17
using ratio_t = std::pair<int, int>;

int main(int argn, char** args);

access_type load_file(static_string_t filename);

std::ostream& operator<< (std::ostream& out, Ops& op);
std::istream& operator>> (std::istream& in,  Ops& op);