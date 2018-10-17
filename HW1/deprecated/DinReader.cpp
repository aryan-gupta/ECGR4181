
#include <string>

#include "DinReader.hpp"

// Every impl I take a look at shows me that there is no way to convert a
// byte buffer into a string stream to parse using op>> without copying.
// So low lever stuff here I come...

DinReader::DinReader(std::string file)
	: mFile{ file }, mBuffer{ new byte_t[BUFF_SZ] }, mLoc{ mBuffer.get() }
{
	nextBuffer();
}

void DinReader::nextBuffer() {
	mFile.read(mBuffer.get(), BUFF_SZ - 1);
	mBuffer[BUFF_SZ - 1] = '\0'; // set last byte to \0 for str parsing
}

std::pair<Ops, uint64_t> DinReader::getLine() {
	return {  };
}