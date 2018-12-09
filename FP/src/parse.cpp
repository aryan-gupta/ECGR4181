
#include <unordered_map>
#include <cstring>
#include <cctype>
#include <functional>
#include <string>
#include <ostream>

#include "parse.hpp"


/// A big warning, I went down a deep, deep rabbit hole with this file. Please just trust
// it works, and I will try to explain it in the best english I can.
// Synopsis: The purpose of this parser was to allow modularity and ease of adding of more options.
//    The core of the parser is the unordered_map arg_map. It maps a option like --option-a and -a
//    to an Option class. The Option class stores the type of option it is, a SWITCH or an ARGUMENT,
//    and a function pointer that can be called to parse and set the argument to the proper field in
//    ParseData. If the Option type is just an SWITCH, the function pointer just returns true or false
// POSIX doc: https://www.gnu.org/software/libc/manual/html_node/Argument-Syntax.html

/// This unordered_map maps a option to its details
/// EX. If we have a --cache-size option with 512 as the argument, we would do this: options["--cache-size"].set(parseData, "512");
///    This would find the function related to "--cache-size" and call it. The function would
///    parse the string "512" because the modifier function is std::atol and set the repective member variable in
///    ParseData data to the number 512
/// If our option does not take a argument, then str will be passed in as an empty string and will not be used
/// each entry has 5 members
/// 1) The option string
/// 2) The type of option (ARGUMENT or SWITCH)
/// 3) The member of ParseData the option sets
/// 4) The modifier function that converts the cstring to the member type (see member 3)
/// 5) The alternative option (short or long option)

// I absulutely hate hate having to make 3 entries for each flag, Im going to have to
// find a better way to do this

/*

constexpr unsigned BIT_CNT = 2;   // BIT COUNTER BITS
constexpr unsigned SIG_BITS = 9; // ADDRESS SIGNIFICANT BITS
constexpr unsigned LCO_BITS = 2; // ADDRESS LOW CUTOFF
constexpr unsigned SFT_BITS = 11; // SHIFT REGISTER BITS
*/
const std::unordered_map<static_string_t, Option> arg_map {
	{ "--predictor",
		{
			ARGUMENT,
			get_setter(&ParseData::predictor, [](const char* str) {
				// Ya Imma do this
				if (std::tolower(str[6]) == 't') return Predictor::ALWAYST;
				if (std::tolower(str[6]) == 'n') return Predictor::ALWAYSN;
				if (std::tolower(str[0]) == 'o') return Predictor::ONE_BIT;
				if (std::tolower(str[0]) == 't') return Predictor::TWO_BIT;
				if (std::tolower(str[1]) == 'l') return Predictor::GLOBAL;
				if (std::tolower(str[2]) == 'h') return Predictor::GSHARE;
				if (std::tolower(str[2]) == 'e') return Predictor::GSELECT;
				if (std::tolower(str[0]) == 'l') return Predictor::LOCAL;
				throw bad_prgm_argument{ concact("[E] ", str, " is not a valid predictor") };
			})
		}
	},
	{ "-p",
		{
			ARGUMENT,
			get_setter(&ParseData::predictor, [](const char* str) { // One day I will find a better way to do this
				// Ya Imma do this
				if (std::tolower(str[0]) == 'a' and std::tolower(str[6]) == 't') return Predictor::ALWAYST;
				if (std::tolower(str[0]) == 'a' and std::tolower(str[6]) == 'n') return Predictor::ALWAYSN;
				if (std::tolower(str[0]) == 'o') return Predictor::ONE_BIT;
				if (std::tolower(str[0]) == 't') return Predictor::TWO_BIT;
				if (std::tolower(str[1]) == 'l') return Predictor::GLOBAL;
				if (std::tolower(str[2]) == 'h') return Predictor::GSHARE;
				if (std::tolower(str[2]) == 'e') return Predictor::GSELECT;
				if (std::tolower(str[0]) == 'l') return Predictor::LOCAL;
				throw bad_prgm_argument{ concact("[E] ", str, " is not a valid predictor") };
			})
		}
	},

	{ "--sat-bits",
		{
			ARGUMENT,
			get_setter(&ParseData::saturation_bits, strb2pf2ul)
		}
	},

	{ "--sig-bits",
		{
			ARGUMENT,
			get_setter(&ParseData::significant_bits, strb2pf2ul)
		}
	},

	{ "--sig-lco-bits",
		{
			ARGUMENT,
			get_setter(&ParseData::sig_lco_bits, strb2pf2ul)
		}
	},

	{ "--shiftreg-bits",
		{
			ARGUMENT,
			get_setter(&ParseData::shift_reg_bits, strb2pf2ul)
		}
	},

	{ "--lhr-bits",
		{
			ARGUMENT,
			get_setter(&ParseData::lhr_bits, strb2pf2ul)
		}
	},

	{ "--addr-bits",
		{
			ARGUMENT,
			get_setter(&ParseData::addr_bits, strb2pf2ul)
		}
	},

	{ "--file",
		{
			ARGUMENT,
			get_setter(&ParseData::file, [](const char* str) { return str; })
		}
	},
	{ "-f",
		{
			ARGUMENT,
			get_setter(&ParseData::file, [](const char* str) { return str; })
		}
	},

	{ "-",
		{
			SWITCH,
			get_setter(&ParseData::use_stdin, [](const char* str) { return true; })
		}
	},
};

unsigned long strb2pf2ul(static_string_t str) {
	unsigned long base = std::atol(str.data());
	unsigned long mul = 1;

	char lastChar = str[str.size() - 1];
	if (!std::isdigit(lastChar)) {
		switch (std::tolower(lastChar)) {
			case 'k': mul = 1024UL; break;
			case 'm': mul = 1024UL * 1024; break;
			case 'g': mul = 1024UL * 1024 * 1024; break;
			// case 't': mul = 1024UL * 1024 * 1024 * 1024; break;
			// case 'p': mul = 1024UL * 1024 * 1024 * 1024 * 1024; break; // yep petabyte of cache
			// case 'e': mul = 1024UL * 1024 * 1024 * 1024 * 1024 * 1024; break; // I dont even know at this point
			default: {
				char msg[] = "[E] Bad argument postfix: ?";
				msg[26] = lastChar;
				throw bad_prgm_argument{ std::string{ msg } };
			}
		}
	}

	return base * mul;
}

void parse(ParseData& ret, int argn, const_cstr_array_t args) {
	for (int i = 1; i < argn; ++i) {
		static_string_t str = args[i];
		auto short_arg_loc = arg_map.find( static_string_t{ str.data(), 2 } );

		if (str[0] != '-') continue; // skip the prgm name if there is

		// `--` stops the parsing of cli arguments
		if (str[0] == '-' and str[1] == '-' and str[2] == '\0') break;

		// parse stdin
		if (str[0] == '-' and str[1] == '\0') arg_map.at(str).set(&ret, "");

		// parse long arguments and switches
		else if (str[1] == '-') {
			static_string_t option, argument;

			if (arg_map.find(str) != arg_map.end()) {
				if (arg_map.at(str).type == SWITCH) { // parse as `--option-a` (SWITCH)
					option = str;
					argument = "";
				} else { // parse as `--option-a argument`
					option = str;
					if (i + 1 == argn or args[i + 1][0] == '-') throw bad_prgm_argument{ concact("[E] Missing argument for ", str) };
					argument = args[i + 1];
				}
			} else { // parse as `--option-a=argument`
				size_t loc = str.find('=');
				if (loc == static_string_t::npos) throw bad_prgm_argument{ concact("[E] Bad argument: ", str) };
				option = static_string_t{ str.data(), loc };
				argument = static_string_t{ str.data() + loc + 1 };
			}

			try {
				arg_map.at(option).set(&ret, argument);
			} catch (std::out_of_range& e) {
				throw bad_prgm_argument{ concact("[E] Bad argument: ", option) };
			}
		}

		// parse short arguments
		else if (short_arg_loc != arg_map.end() and short_arg_loc->second.type == ARGUMENT) {
			static_string_t option, argument;

			if (str.length() > 2) { // parse as -aargument
				option = static_string_t{ str.data(), 2 };
				argument = static_string_t{ str.data() + 2 };
			} else { // parse as -a argument
				option = str;
				char msg[] = "[E] Missing argument for -?";
				msg[26] = str[1];
				if (i + 1 == argn or args[i + 1][0] == '-') throw bad_prgm_argument{ msg };
				argument = args[i + 1];
			}

			try {
				arg_map.at(option).set(&ret, argument);
			} catch (std::out_of_range& e) {
				char msg[] = "[E] Bad argument: -?";
				msg[19] = option[1];
				throw bad_prgm_argument{ msg };
			}
		}

		// parse short switches
		else if (short_arg_loc != arg_map.end() and short_arg_loc->second.type == SWITCH) {
			size_t loc = 1; // the switches start at 1 (`-abc`)
			do {
				char option[3]{ '-', str[loc], '\0' };

				if (arg_map.find(option) == arg_map.end()) {
					char msg[] = "[E] Bad argument: -?";
					msg[19] = option[1];
					throw bad_prgm_argument{ std::string{ msg } };
				}

				if (arg_map.at(option).type != SWITCH) {
					char msg[] = "[E] -? requires a argument";
					msg[5] = option[1];
					throw bad_prgm_argument{ std::string{ msg } }; // need to convert msg to string because it is no longer const static??
				}

				arg_map.at(option).set(&ret, static_string_t{});

			} while (++loc < str.length());
		}

		else {
			throw bad_prgm_argument{ concact("[E] Bad argument: ", str) };
		}
	}
}
