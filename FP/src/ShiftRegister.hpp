
#pragma once

#include <cstdint>
#include <cmath>
#include <limits>

#include "main.hpp"

// Please see BitCounter.hpp to see why this code blew up so large


namespace detail {

struct ShiftRegisterBase {
	virtual bool operator>> (bool in) = 0;
	virtual bool operator<< (bool in) = 0;
	virtual uint64_t value() = 0;
	virtual unsigned bits() = 0;
};

template <typename T>
class ShiftRegisterImpl : public ShiftRegisterBase {
	T mHiMask;
	T mLoMask;
	T mValue;

public:
	ShiftRegisterImpl(unsigned bits, T init = 0)
		: mHiMask( 1 << (bits - 1) ) // Imma use the parenthesisi to stop from narrowing conversions
		, mLoMask( std::numeric_limits<T>::max() >> ((sizeof(T) * 8) - bits + 1) )
		, mValue{ init } {  }

	virtual bool operator>> (bool in) {
		bool out = mValue & 1; // get value that will be shifted out
		mValue >>= 1; // shift it out
		if (in) mValue |= mHiMask; // shift in the new value
		return out;
	}

	virtual bool operator<< (bool in) {
		bool out = mValue & mHiMask;
		mValue &= mLoMask; // strip all but LSB
		mValue <<= 1;
		mValue |= in;
		return out;
	}

	virtual uint64_t value() {
		return mValue;
	}

	virtual unsigned bits() {
		return (unsigned)std::log2(mHiMask) + 1;
	}
};

} // end namespace detail


class ShiftRegister {
	detail::ShiftRegisterBase* mImpl;

public:
	ShiftRegister(unsigned bits, uint64_t init = 0);

	ShiftRegister(const ShiftRegister& clone);

	bool operator>> (bool in);

	bool operator<< (bool in);

	uint64_t value();

};