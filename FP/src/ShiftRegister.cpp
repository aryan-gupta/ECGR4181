

#include "ShiftRegister.hpp"

ShiftRegister::ShiftRegister(unsigned bits, uint64_t init) {
	if      (bits <=  8) mImpl = new detail::ShiftRegisterImpl< uint8_t>{ bits, ( uint8_t)init };
	else if (bits <= 16) mImpl = new detail::ShiftRegisterImpl<uint16_t>{ bits, (uint16_t)init };
	else if (bits <= 32) mImpl = new detail::ShiftRegisterImpl<uint32_t>{ bits, (uint32_t)init };
	else if (bits <= 64) mImpl = new detail::ShiftRegisterImpl<uint64_t>{ bits, (uint64_t)init };
	else throw std::invalid_argument{ "[E] Shift Register larger than 128 bits currently not supported" };
}


ShiftRegister::ShiftRegister(const ShiftRegister& clone)
	: ShiftRegister{ clone.mImpl->bits(), clone.mImpl->value() } {  }

bool ShiftRegister::operator>> (bool in) {
	return (*mImpl) >> in;
}

bool ShiftRegister::operator<< (bool in) {
	return (*mImpl) << in;
}

uint64_t ShiftRegister::value() {
	return mImpl->value();
}