

#include "BitCounter.hpp"

BitCounter::BitCounter(unsigned bits, uint64_t init) {
	using namespace detail;
	// vscode and me are having a serious love/hate relationship going
	// issue report opened https://github.com/Microsoft/vscode-cpptools/issues/2907
	if      (bits <=   8) mImpl = new BitCounterImpl< uint8_t>{ bits, (uint8_t)init };
	else if (bits <=  16) mImpl = new BitCounterImpl<uint16_t>{ bits, (uint16_t)init };
	else if (bits <=  32) mImpl = new BitCounterImpl<uint32_t>{ bits, (uint32_t)init };
	else if (bits <=  64) mImpl = new BitCounterImpl<uint64_t>{ bits, (uint64_t)init };
	else throw std::invalid_argument{ "[E] Bit Counter larger than 128 bits currently not supported" };
}

BitCounter::BitCounter(const BitCounter& clone)
	: BitCounter{ clone.mImpl->bits(), clone.mImpl->value() } {  }

BitCounter::~BitCounter() {
	delete mImpl;
}

BitCounter& BitCounter::operator= (const BitCounter& other) {
	// `delete this' is wrong. It will also delete the pointer variable
	this->~BitCounter(); // destroy us
	new (this) BitCounter{ other.mImpl->bits(), other.mImpl->value() }; // construct us with params
	return *this;
}


BitCounter& BitCounter::operator++ () {
	++(*mImpl);
	return *this;
}

BitCounter& BitCounter::operator-- () {
	--(*mImpl);
	return *this;
}

BitCounter BitCounter::operator++ (int) {
	BitCounter orig{ *this };
	++(*mImpl);
	return orig;
}

BitCounter BitCounter::operator-- (int) {
	BitCounter orig{ *this };
	--(*mImpl);
	return orig;
}

uint64_t BitCounter::value() const {
	return mImpl->value();
}

size_t BitCounter::max() const {
	return mImpl->max();
}