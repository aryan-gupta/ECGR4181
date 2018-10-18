
#pragma once

#include <iostream>

struct printer {
	static inline void dot() {
		std::cout << ".";
	}

	static inline void bksp() {
		std::cout << "\b \b";
	}

	static inline void intro() {
		/// from ping(8):
		// For every ECHO_REQUEST sent a period “.” is printed, while for ever ECHO_REPLY received a backspace is
		// printed. This provides a rapid display of how many packets are being dropped.
		// Also just realized they misspelled every
		// I fell like there will be a point where the period is spammed so I may represent = by 10 periods? idk
		std::cout << "File and parameters successfully loaded." "\n"
					"For every memory access a period \".\" is printed, while for every memory hit a backspace is "
					"printed, this provides a rapid display of how many cache misses there are" "\n";
	}

};