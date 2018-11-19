
#pragma once

#include <cstdint>
#include <vector>
#include <iostream>
#include <type_traits>

// constexpr

using addr_t = uint32_t; // type for storing addresses
using trace_t = std::vector<std::pair<addr_t, bool>>;
using ratio_t = std::pair<int, int>;


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

template <std::size_t S>
using uleast_t = typename uleast<S>::type;