

/// After

/// A very fast and crude implementation for a memory compressed
/// boolean array. Please note that std::vector already has this
/// impl, but I needed a stack alloc array

#include <type_traits>
#include <iterator>

#include <vector>

namespace ari { // extends https://github.com/aryan-gupta/libari

//// THIS IS INVALID CODE AND WILL NOT COMPILE
// 	std::vector<bool> test;
// 	test.push_back(true);
// 	bool& ref = test[0];

template <typename T>
class bool_array_proxy {
public:
	using base_type = T;
	using value_type = bool;
	using reference = base_type&;
	// index is the number of 8 * sizeof(base_type). Lets us use the base_type as a array of bits
	using index_type = unsigned short;
	using mask_type = base_type; // lets us mask out the value

private:
	using self = bool_array_proxy<T>; // stolen from python (hence private)

	reference mRef;
	mask_type mMask;

public:
	bool_array_proxy() = delete; // cant be default init
	constexpr bool_array_proxy(reference ref, index_type idx) : mRef{ ref }, mMask{ 1 } {
		mMask <<= idx;
	}

	operator bool() const {
		return mRef |  mMask; // if our mask returns the value at the index and casts true if >0
	}

	self operator= (bool) {

	}
};

template <typename T, std::size_t N, typename = std::enable_if_t<std::is_same_v<T, bool>>> // lock for bool, need to change later
class
[[deprecated("ari::array<bool> development was discontinued upon discvery of std::bitset")]]
array {
public:
	using value_type = T;
	using array_base_type = std::byte;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using reference = bool_array_proxy<array_base_type>;
	using const_reference = bool;
	/// @warning No iterator support yet

private:
	constexpr static size_type mSize = N / (sizeof(array_base_type) * 8);
	array_base_type mData[mSize];

public:
	// Ill let the compiler do the aggrigate inits and ctors/dtors
	reference at(size_type idx) {
		return { *mData, 0 };
	}

};

}
