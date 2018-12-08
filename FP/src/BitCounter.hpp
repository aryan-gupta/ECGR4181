
#pragma once

#include <cstdint>
#include <type_traits>
#include <iostream>
#include <cmath>

#include "main.hpp"

// A simple bit counter. Counts up and down from 0 to the max (2 ^ B). Converted to a runtime
// BitCounter. The original idea was to use an array of 8bit ints but it would get complicated. 
// So I chose to just use an max width int and leave it at that Counter does not look, therefore,
// doing ++BitCounter{ mSize - 1 } will do nothing. Essentally, a 2 bit counter can only have values of 0, 1, 2, and 3.
class BitCounter {
	using base_t = uint64_t;
	using rawval_t = uint64_t;

	static constexpr unsigned MAX_BITS = (sizeof(base_t) * 8) - 1;

	base_t mMax;
	base_t mCount;

public:
	BitCounter(unsigned bits);

	BitCounter& operator++ ();

	BitCounter& operator-- ();

	BitCounter operator++ (int);

	BitCounter operator-- (int);

	// Im so lazy to so the other comparison operators and my compiler doesnt support <=> operator
	// yet so, Imma do this
	rawval_t value() const;

	static std::size_t max();
};