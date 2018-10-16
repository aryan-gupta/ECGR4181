
#include <string>
#include <utility>
#include <fstream>
#include <sstream>
#include <vector>

#include "main.hpp"

class
[[deprecated("Unused class `DinReader` is depreciated")]]
DinReader {
	using byte_t = char;
	using buffer_t = std::unique_ptr<byte_t[]>;

	constexpr static size_t BUFF_SZ = 1024 * 1024; // 1MB buffer

	std::ifstream mFile;
	buffer_t mBuffer;
	byte_t* mLoc;

	void nextBuffer();

public:
	DinReader() = default;
	DinReader(std::string file);

	std::pair<Ops, uint64_t> getLine();
};