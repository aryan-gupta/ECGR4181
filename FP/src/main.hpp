
#pragma once

#include <cstdint>
#include <vector>
#include <iostream>
#include <type_traits>

using addr_t = uint32_t; // type for storing addresses
using trace_t = std::vector<std::pair<addr_t, bool>>;
using ratio_t = std::pair<int, int>;

constexpr unsigned BIT_CNT = 2;   // BIT COUNTER BITS
constexpr unsigned SIG_BITS = 9; // ADDRESS SIGNIFICANT BITS
constexpr unsigned LCO_BITS = 2; // ADDRESS LOW CUTOFF
constexpr unsigned SFT_BITS = 11; // SHIFT REGISTER BITS


template <typename T, typename = std::enable_if_t<std::is_base_of<std::istream, typename std::decay_t<T>>::value>>
trace_t load_stream(T&& stream) {
	if (!stream) throw std::invalid_argument{ "[E] stream cannot be empty" };

	trace_t ret_val;

	for (std::string line; std::getline(stream, line); ) {
		addr_t addr = std::stoll(line);
		bool taken = (line[line.size() - 1] == 'T')? true : false;

		ret_val.push_back({ addr, taken });
	}

	return ret_val;
}

enum class Predictor {
	ALWAYST,
	ALWAYSN,
	ONE_BIT,
	TWO_BIT,
	GLOBAL,
	GSHARE,
	GSELECT,
	LOCAL,
	CUSTOM
};

// This code picks a uint type that is at least X bits. For example,
// if we run uleast_t<5> then it will return uint8_t because it is at least
// 5 bits. If we run uleast_t<9> then it will return uint16_t because it is
// at least 9 bytes
template <std::size_t A, std::size_t B>
constexpr bool less() {
	return A < B;
}

template <std::size_t S>
struct uleast {
	using type =
		std::conditional_t<less<S, 8>(), uint8_t,
			std::conditional_t<less<S, 16>(), uint16_t,
				std::conditional_t<less<S, 32>(), uint32_t,
					std::conditional_t<less<S, 64>(), uint64_t,
						void
					>
				>
			>
		>;
};

// std::pow is not yet constexpr
template <typename T, typename U, typename = std::enable_if_t<std::is_integral<U>::value>>
constexpr auto pow(T b, U e) -> T {
	return (e == 0)? 1 : b * pow(b, e - 1);
}

template <std::size_t S>
using uleast_t = typename uleast<S>::type;

void print_help();

std::ostream& operator<< (std::ostream& out, Predictor op);