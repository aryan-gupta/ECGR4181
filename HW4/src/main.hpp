
#pragma once

#include <cstdint>
#include <vector>

// constexpr

using addr_t = uint32_t; // type for storing addresses
using trace_t = std::vector<std::pair<addr_t, bool>>;
using ratio_t = std::pair<int, int>;