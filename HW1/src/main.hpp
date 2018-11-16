/*
 * The starting point of the program and other functions that dont fit in
 * at other places
 */

#pragma once

#include <memory>
#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <type_traits>
#include <sstream>

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

using ptr_t = uint64_t;
using access_type = std::vector<std::pair<Ops, ptr_t>>;

template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>> // std::is_integral_v<T> is c++17
using ratio_t = std::pair<int, int>;

int main(int argn, char** args);

std::ostream& operator<< (std::ostream& out, Ops& op);
std::istream& operator>> (std::istream& in,  Ops& op);


// I need the decay here because if I do load_stream(std::cin) then
// T is a std::istream& and std::istream and std::istream& are not the
// same or base type. Decay will transform std::istream& to std::istream
// Also we cant use std::is_base_of_v as it is c++17
template <typename T, typename = std::enable_if_t<std::is_base_of<std::istream, typename std::decay_t<T>>::value>>
access_type load_stream(T&& stream) {
	if(!stream) return { };

	std::vector<std::pair<Ops, ptr_t>> ret;

	for (std::string line{  }; std::getline(stream, line); ) {
		std::stringstream ss{ line };
		Ops code;
		ptr_t location;

		ss >> code >> std::hex >> location >> std::dec;

		if (ss.bad()) return ret;
		if (ss.fail()) return ret;

		ret.emplace_back(code, location);
	}

	return ret;
}