
#include <cstdint>
#include <limits>

#include "main.hpp"

template <std::size_t B, typename T = uleast_t<B>>
class ShiftRegister {
	constexpr T mMask = 1 << (B - 1);
	T mValue;

public:
	constexpr bool operator>> (bool in = false) {
		bool out = mValue & 1; // get value that will be shifted out
		mValue >>= 1; // shift it out
		if (in) mValue ^= mMask; // shift in the new value
		return out;
	}

	constexpr bool operator<< (bool in = false) {
		constexpr T mask = std::numeric_limits<T>::max() >> ((sizeof(T) * 8) - 10 + 1);
		bool out = mValue & mMask;
		mValue &= mask; // strip all but LSB
		mValue <<= 1;
		if (in) mValue ^= 1;
		return out;
	}
};