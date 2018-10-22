
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
const std::unordered_map<static_string_t, Option> arg_map {
	{ "--cache-size",
		{
			ARGUMENT,
			get_setter(&ParseData::cache_size, strb2pf2ul),
			"-c"
		}
	},
	{ "-c",
		{
			ARGUMENT,
			get_setter(&ParseData::cache_size, strb2pf2ul),
			"--cache-size"
		}
	},

	{ "--block-size",
		{
			ARGUMENT,
			get_setter(&ParseData::block_size, strb2pf2ul),
			"-b"
		}
	},
	{ "-b",
		{
			ARGUMENT,
			get_setter(&ParseData::block_size, strb2pf2ul),
			"--block-size"
		}
	},

	{ "--replace-policy",
		{
			ARGUMENT,
			get_setter(&ParseData::replace_policy, [](const char* str){ return str; }),
			"-r"
		}
	},
	{ "-r",
		{
			ARGUMENT,
			get_setter(&ParseData::replace_policy, [](const char* str){ return str; }),
			"--replace-policy"
		}
	},

	{
		"--associativity",
		{
			ARGUMENT,
			get_setter(&ParseData::associativity, std::atol),
			"-a"
		}
	},
	{ "-a",
		{
			ARGUMENT,
			get_setter(&ParseData::associativity, std::atol),
			"--associativity"
		}
	},

	{ "--file",
		{
			ARGUMENT,
			get_setter(&ParseData::file, [](const char* str){ return str; }),
			"-f"
		}
	},
	{ "-f",
		{
			ARGUMENT,
			get_setter(&ParseData::file, [](const char* str){ return str; }),
			"--file"
		}
	},

	{ "-",
		{
			SWITCH,
			get_setter(&ParseData::use_stdin, [](const char* str){ return true; }),
			"-"
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

ParseData parse(const_cstr_array_t args, int argn) {
	ParseData ret{
		.file = "ECGR4181/HW1/project/trace.din",
		.replace_policy = "FIFO",
		.cache_size = 32 * 1024, // ~65KB
		.block_size = 128,   // =255B
		.associativity = 2,
		.use_stdin = false
	};

	for (int i = 1; i < argn; ++i) {
		static_string_t str = args[i];
		auto short_arg_loc = arg_map.find( static_string_t{ str.data(), 2 } );

		if (str[0] != '-') continue; // skip the prgm name if there is

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

	return ret;
}
