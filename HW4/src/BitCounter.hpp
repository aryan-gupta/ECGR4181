
#pragma once

#include <cstdint>
#include <type_traits>
#include <iostream>

// std::pow is not yet constexpr
template <typename T, typename U, typename = std::enable_if_t<std::is_integral<U>::value>>
constexpr auto pow(T b, U e) -> T {
	return (e == 0)? 1 : b * pow(b, e - 1);
}

// A simple bit counter. Counts up and down from 0 to the max (2 ^ B). There is probs a way
// to remove the typename T but Im trying to get this working. Will try if I get time. The
// Counter does not look, therefore, doing ++BitCounter{ mSize - 1 } will do nothing. Essentally,
// a 2 bit counter can only have values of 0, 1, 2, and 3.
template <std::size_t B, typename T>
class BitCounter {
	static constexpr std::size_t mSize = pow(2, B);
	T mCount;

public:
	constexpr BitCounter() : mCount{ max() / 2 } {  }
	constexpr BitCounter(T count) : mCount{ count } {  }

	constexpr BitCounter& operator++ () {
		// increment mCount and if we are at max then normalize it
		if (++mCount == mSize) mCount = mSize - 1;
		return *this;
	}

	constexpr BitCounter& operator-- () {
		if (mCount != 0) --mCount;
		return *this;
	}

	constexpr BitCounter operator++ (int) {
		T orig = mCount;
		if (++mCount == mSize) mCount = mSize - 1;
		return BitCounter{ orig };
	}

	constexpr BitCounter operator-- (int) {
		T orig = mCount;
		if (mCount != 0) --mCount;
		return BitCounter{ orig };
	}

	// Im so lazy to so the other comparison operators and my compiler doesnt support <=> operator
	// yet so, Imma do this
	constexpr T value() const {
		return mCount;
	}

	static constexpr std::size_t max() {
		return mSize;
	}

};