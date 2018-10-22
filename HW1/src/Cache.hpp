/*
 * This class represents a cache, this should allow modularity for
 * multiple caches (like L1, L3, etc)
 */


#pragma once

#include <cmath>
#include <cassert>
#include <limits>

#include "main.hpp"
#include "parse.hpp"

// Our cache size is the `size of the cache / cache block size`
// We we will only store the starting address of the cache block
// for example. If we want to access 0x341F3FF, we will store
// 0x341F300 if each cache block is 255 bytes

// If we have a 32K cache and 1K block size then we have 32 indecies
// If we have a 4G cache and 1B block then we would have 0xFFFFFFFF (4G)
// This means we need a 32bit value max to represent each cache address
// !! Main Memory is 4G !!

// Parts of an address
//  Tag -    The bits that identify an unique address in memory.
//           The tag is used to check if our memory address is located
//           in our cache or not. If the tab is not the same, then we have
//           cache miss.
//  Index -  The bits that define where in the cache the memory is located.
//           If the index is 0 then the data will be in the 1st cache location
//           or we have a cache miss (cache size / block size)
//  Offset - The ofset in the cache block where the byte is located. Is used
//           when we have a block size larger than 1B.
// The index tells us where the memory location would be in the cache, the tag
// is used to verify that we have a cache hit/miss and the offset tells us
// where in the cache block the byte is

// @todo make this into seperate arrays, because the valid flag can be compressed
struct cache_info {
	bool valid = false;
	ptr_t tag;
};

// These variables must be set because we cant use a packed bitfield
// a bitfield is a compiler-time derivative but our sizes are all runtime
struct addr_info {
	size_t Tag_Offset;
	size_t Index_Mask;
	size_t Index_Offset;
	size_t Offset_Mask;
};

#if __cpp_constexpr >= 201603
constexpr
#else
#warning "c++17 `__cpp_constexpr' not fully supported, falling back to inline/runtime impl"
inline
#endif
auto get_info(ParseData pd) -> addr_info {
	size_t block_bits = std::log2(pd.block_size);
	size_t index_bits = std::log2(pd.cache_size / pd.block_size / pd.associativity);

	constexpr int MAX_BIT = sizeof(ptr_t) * 8;
	constexpr ptr_t F_MASK = std::numeric_limits<ptr_t>::max(); // creates 0xFFF...
	addr_info ret{};
	ret.Offset_Mask = F_MASK >> (MAX_BIT - block_bits);
	ret.Index_Mask = F_MASK >> (MAX_BIT - index_bits - block_bits);
	ret.Index_Offset = block_bits;
	ret.Tag_Offset = index_bits + block_bits;
	return ret;
}

using locate_func_t = std::function<bool(cache_info*, ptr_t)>;
using replace_func_t = std::function<void(cache_info*, ptr_t)>;

void always_replace_policy(cache_info* cache, ptr_t idx, ptr_t tag);

replace_func_t get_replace_func(uint8_t assoc, size_t offset);
locate_func_t get_locate_func(uint8_t assoc, size_t cache_size);

class Cache {
	// Please note that using a unordered_map would reduce usage of memory alot
	// but would take off the spirit of the assignment
	const addr_info mAddrInfo;
	cache_info* mCache;

	Cache* mNext;

	int mHits;
	int mAccess;

	replace_func_t mReplace;
	locate_func_t mLocate;

	ptr_t getTag(ptr_t ptr);
	ptr_t getIndex(ptr_t ptr);
	ptr_t getOffset(ptr_t ptr);

public:
	Cache() = delete;
	Cache(const ParseData& pd);
	~Cache();

	void read(ptr_t addr);
	void write(ptr_t addr);
	void fetch(ptr_t addr);

	int getHits() const;
	int getAccess() const;

};