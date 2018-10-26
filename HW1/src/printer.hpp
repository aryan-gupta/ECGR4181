/*
 * This class was soly made so I could move all the std::cout << .... to a seperate file
 * so the other files are cluttered with multiple lines of std::cout's. The functions are
 * pretty self explanitory, and/or are easy to read so this file will not have any docs
 */
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
		std::cout << "File and parameters successfully loaded." << std::endl << std::endl;

		if (pd.uni) {
			std::cout << "Unified Cache size...... " << pd.uni_cache_size << std::endl;
			std::cout << "Unified Block size...... " << pd.uni_block_size << std::endl;
			std::cout << "Unified Replace Policy.. " << pd.uni_replace_policy << std::endl;
			std::cout << "Unified Associativity... " << (int)pd.uni_associativity << std::endl;

			std::cout << "Use stdin............... " << pd.use_stdin << std::endl;
		} else {
			std::cout << "Data Cache size............. " << pd.dat_cache_size << std::endl;
			std::cout << "Data Block size............. " << pd.dat_block_size << std::endl;
			std::cout << "Data Replace Policy......... " << pd.dat_replace_policy << std::endl;
			std::cout << "Data Associativity.......... " << (int)pd.dat_associativity << std::endl;

			std::cout << "Instruction Cache size...... " << pd.uni_cache_size << std::endl;
			std::cout << "Instruction Block size...... " << pd.uni_block_size << std::endl;
			std::cout << "Instruction Replace Policy.. " << pd.uni_replace_policy << std::endl;
			std::cout << "Instruction Associativity... " << (int)pd.uni_associativity << std::endl;

			std::cout << "Use stdin................... " << pd.use_stdin << std::endl;
		}

		// std::cout << "For every memory access a period \".\" is printed, while for every memory hit a backspace is "
		// 			 "printed, this provides a rapid display of how many cache misses there are" << std::endl;
	}

	static inline void result(const Simulator& sim) {
		std::cout << std::endl; // flush buffer and ouput new line
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
				if (value.type == ARGUMENT)
					std::cout << "  <value>";
				std::cout << std::endl;
			}
		}
	}

};