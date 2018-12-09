
#pragma once

#include <cstdint>
#include <type_traits>
#include <iostream>
#include <cmath>

#include "main.hpp"

// This is yet another class that started out very simple and now is a convuluted
// complex work of art. Here I go explaining it. There is a Wrapper class called
// BitCounter. This stores an BitCounterBase Pointer that is an abstract class
// that references a BitCounterImpl<T> class. The power of polymorphism on template
// classes. Because of the virtual addr table, the function calls will be indirect
// and slightly slower

namespace detail {

struct BitCounterBase {
	virtual ~BitCounterBase() = default;
	virtual BitCounterBase* operator++ () = 0;
	virtual BitCounterBase* operator-- () = 0;
	virtual uint64_t value() const = 0;
	virtual size_t max() const = 0;
	virtual unsigned bits() const = 0;
};

// A simple bit counter. Counts up and down from 0 to the max (2 ^ B). There is probs a way
// to remove the typename T but Im trying to get this working. Will try if I get time. The
// Counter does not look, therefore, doing ++BitCounterImpl{ mSize - 1 } will do nothing. Essentally,
// a 2 bit counter can only have values of 0, 1, 2, and 3.
template <typename T>
class BitCounterImpl : public BitCounterBase {
	std::size_t mSize;
	T mCount;

public:
	BitCounterImpl(unsigned bits, T init = 0)
		: mSize{ (unsigned)pow(2, bits) }
		, mCount{ init }
		{  }

	virtual ~BitCounterImpl() = default;

	virtual BitCounterBase* operator++ () {
		// increment mCount and if we are at max then normalize it
		if (++mCount == mSize) mCount = mSize - 1;
		return this;
	}

	virtual BitCounterBase* operator-- () {
		if (mCount != 0) --mCount;
		return this;
	}

	// Im so lazy to so the other comparison operators and my compiler doesnt support <=> operator
	// yet so, Imma do this
	// There is a way to do this using type erasure. Will figure it out later
	virtual uint64_t value() const {
		return mCount;
	}

	virtual size_t max() const {
		return mSize;
	}

	virtual unsigned bits() const {
		return (unsigned)std::log2(mSize);
	}

};

} // end namespace detail

class BitCounter {
	detail::BitCounterBase* mImpl;

public:
	BitCounter(unsigned bits, uint64_t init = 0);

	BitCounter(const BitCounter& clone);

	~BitCounter();

	BitCounter& operator= (const BitCounter& other);

	BitCounter& operator++ ();

	BitCounter& operator-- ();

	BitCounter operator++ (int);

	BitCounter operator-- (int);

	uint64_t value() const;

	size_t max() const;

};