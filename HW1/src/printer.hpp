
#pragma once

#include <iostream>

#include "parse.hpp"
#include "Simulator.hpp"

struct printer {
	static inline void dot() {
		//std::cout << ".";
	}

	static inline void bksp() {
		//std::cout << "\b \b";
	}

	static inline void intro(const ParseData& pd) {
		/// from ping(8):
		// For every ECHO_REQUEST sent a period “.” is printed, while for ever ECHO_REPLY received a backspace is
		// printed. This provides a rapid display of how many packets are being dropped.
		// Also just realized they misspelled every
		// I fell like there will be a point where the period is spammed so I may represent = by 10 periods? idk
		std::cout << "File and parameters successfully loaded." << std::endl;

		std::cout << "Cache size...... " << pd.cache_size << std::endl;
		std::cout << "Block size...... " << pd.block_size << std::endl;
		std::cout << "Use stdin....... " << pd.use_stdin << std::endl;
		std::cout << "Replace Policy.. " << pd.replace_policy << std::endl;
		std::cout << "Associativity... " << (int)pd.associativity << std::endl;

		std::cout << "For every memory access a period \".\" is printed, while for every memory hit a backspace is "
					 "printed, this provides a rapid display of how many cache misses there are" << std::endl;
	}

	static inline void result(const Simulator& sim) {
		std::cout << std::endl << std::endl; // flush buffer and ouput new line
		std::cout << "================ Results ================\n";
		auto ratio = sim.getRatio();
		double d = ratio.first / (double)ratio.second;
		std::cout << "Hits: " << ratio.first << " "
		          << "Misses: " << (ratio.second - ratio.first) << " "
		          << "Total: " << ratio.second << " "
		          << "Ratio: " << d << '\n';
	}

	static inline void help() {
#if __cpp_structured_bindings
		for (auto [key, value] : arg_map) {
#else
		for (auto& _p : arg_map) {
			auto key = _p.first; auto value = _p.second;
#endif
			if (key[1] == '-') { // found long option
				std::cout << key;
				std::cout << " (" << value.alt_op << ')';
				if (value.type == ARGUMENT)
					std::cout << "  <value>";
				std::cout << std::endl;
			}
		}
	}

};