// Copyright (c) 2016-2017 Matthias Noack <ma.noack.pr@gmail.com>
//
// See accompanying file LICENSE and README for further information.

#include "noma/num/rk_method.hpp"

#include <string>

#include <noma/typa/parser_error.hpp>

namespace noma {
namespace num {

std::ostream& operator<<(std::ostream& out, const rk_method_t& m)
{
	out << rk_method_names.at(m);
	return out;
}

std::istream& operator>>(std::istream& in, rk_method_t& m)
{
	std::string value;
	std::getline(in, value);

	// get key to value
	// NOTE: we trust rk_method_names to be complete here
	bool found = false;
	for (auto it = rk_method_names.begin(); it != rk_method_names.end(); ++it)
		if (it->second == value) {
			m = it->first;
			found = true;
			break;
		}

	if (!found)
		throw noma::typa::parser_error("'" + value + "' is not a valid rk_method.");

	return in;
}

} // namespace num
} // namespace noma
