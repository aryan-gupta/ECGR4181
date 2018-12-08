
#include "BitCounter.hpp"

BitCounter::BitCounter(unsigned bits)
	: mMax{ pow(2, bits) }, mCount{ 0 }  { 
	if (bits > MAX_BITS) throw std::invalid_argument{ "[E] BitCounters larger than " + std::to_string(MAX_BITS) + " bits are currently not supported" };
}

BitCounter& BitCounter::operator++() {
	// increment mCount and if we are at max then normalize it
	if (++mCount == mMax) mCount = mMax - 1;
	return *this;
}

BitCounter& BitCounter::operator-- () {
	if (mCount != 0) --mCount;
	return *this;
}

BitCounter BitCounter::operator++ (int) {
	T orig = mCount;
	if (++mCount == mMax) mCount = mMax - 1;
	return BitCounter{ orig };
}

BitCounter BitCounter::operator-- (int) {
	T orig = mCount;
	if (mCount != 0) --mCount;
	return BitCounter{ orig };
}

// Im so lazy to so the other comparison operators and my compiler doesnt support <=> operator
// yet so, Imma do this
auto BitCounter::value() const -> rawval_t {
	return mCount;
}

std::size_t BitCounter::max() {
	return mMax;
}