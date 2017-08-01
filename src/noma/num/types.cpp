// Copyright (c) 2016-2017 Matthias Noack <ma.noack.pr@gmail.com>
//
// See accompanying file LICENSE and README for further information.

#include "noma/num/types.hpp"

#include <iostream>
#include <limits>

namespace noma {
namespace num {

int_t get_real_to_text_digits() {
	return std::numeric_limits<real_t>::max_digits10;
}

std::string get_int_format_string()
{
	return "%i";
}

std::string get_real_format_string()
{
	// e.g. "%.17e, where 17 is get_real_to_text_digits()"
	static const std::string result { "%." + (boost::format("%i") %  get_real_to_text_digits()).str() + "e" };
	//std::cout << "get_real_format_string: " << result << std::endl;
	return result;
}

std::string get_complex_format_string()
{
	// e.g. "(%.17e,%.17e)"
	static const std::string result { "(" + get_real_format_string() + "," + get_real_format_string() + ")" };
	//std::cout << "get_complex_format_string: " << result << std::endl;
	return result;
}

const boost::format int_format(get_int_format_string());
const boost::format real_format(get_real_format_string());
const boost::format complex_format(get_complex_format_string());

const char default_delimiter = '\t';

} // namespace num
} // namespace noma
