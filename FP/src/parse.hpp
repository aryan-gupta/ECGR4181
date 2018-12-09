/*
 * This file (and parse.cpp) manages all the logic behind the parsing of command
 * line arguments. It started out very simple but then grew to this monstrcity.
 * More details are in the parse.cpp. But Trying to understand how it all works
 * will be a challenge. Make sure you are very well versed in functional pointers
 */

#pragma once

#include <string>
#include <memory>
#include <cstddef>
#include <functional>
#include <stdexcept>

#if __cpp_lib_string_view
	#include <string_view>
	using static_string_t = std::string_view;
#else
	#warning "`__cpp_lib_string_view' not supported, falling back to `std::string'"
	using static_string_t = std::string;
#endif

#include "main.hpp"

/// This function concact multiple strings together
// Fold expressions are c++17 so I need feature test macros
#if __cpp_fold_expressions

template <typename... R>
std::string concact(R... a) {
	return (std::string{ a } + ...);
}

#else
#warning "`__cpp_fold_expressions' not supported, falling back to recursive vardiac template functions"

template <typename T1, typename T2>
std::string concact(T1 a, T2 b) {
	return std::string{ a } + std::string{ b };
}

template <typename T, typename... R>
std::string concact(T a, T b, R... c) {
	return std::string{ a } + concat(b, c...);
}

#endif

struct ParseData {
	static_string_t file;

	Predictor predictor;
	unsigned saturation_bits;
	unsigned significant_bits;
	unsigned sig_lco_bits;
	unsigned shift_reg_bits;
	unsigned lhr_bits;
	unsigned addr_bits;

	bool            use_stdin;
};

class bad_prgm_argument : public std::invalid_argument {
	using invalid_argument::invalid_argument;
};


// Parser set function. These takes a string argument and sets it to the proper member
// variable in ParseData. The function has the option to parse the argument into a int
// or other variable type if need be. Need to be wrapped by std::function so it can
// latch to both func ptrs and lambdas
using parse_setter_t = std::function<void (ParseData*, static_string_t)>;

// Type of options
enum OpType {
	SWITCH,  // takes no extra aruments (boolean -turnon or -turnoff)
	ARGUMENT // takes in an extra argument (--option argument or --option=argument)
};

// A option detail, the type of option it is and its respective setter function
struct Option {
	OpType type;
	parse_setter_t set;
};

// This function returns a function that sets the member variable using the modifyer function func
// EX. if we want to set ParseData::option to 512 then we would call this as get_setter(ParseData::option, std::atol)
// This will return a function like this:
//	 [](ParseData* const pd, static_string_t str) -> void {
//		 pd->option = atol(str);
//	 }
// The returned function can be used to take a string argument and convert it to member of ParseData
// The best way to explain this is this function returns a function that does somthing determined by
// its parameter
template <typename T, typename F>
constexpr parse_setter_t get_setter(T ParseData::* member, F func) {
	return [=](ParseData* const pd, static_string_t str) {
		pd->*member = func(str.data());
	};
}

unsigned long strb2pf2ul(static_string_t str);

extern const std::unordered_map<static_string_t, Option> arg_map;

using const_cstr_array_t = char const* const* const; // const pointer to a const pointer (array) to const char

// Python interface
#ifdef PYTHON_API
extern "C" {
#endif

void parse(ParseData& ret, int argn, const_cstr_array_t args);

#ifdef PYTHON_API
}
#endif