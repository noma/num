// Copyright (c) 2017 Matthias Noack <ma.noack.pr@gmail.com>
//
// See accompanying file LICENSE and README for further information.

#include "noma/num/stepper_type.hpp"

#include <noma/typa/parser_error.hpp>

namespace noma {
namespace num {

std::ostream& operator<<(std::ostream& out, const stepper_type_t& t)
{
	out << stepper_type_names.at(t);
	return out;
}

std::istream& operator>>(std::istream& in, stepper_type_t& t)
{
	std::string value;
	std::getline(in, value);

	// get key to value
	// NOTE: we trust stepper_type_names to be complete here
	bool found = false;
	for (auto it = stepper_type_names.begin(); it != stepper_type_names.end(); ++it)
		if (it->second == value) {
			t = it->first;
			found = true;
			break;
		}

	if (!found)
		throw noma::typa::parser_error("'" + value + "' is not a valid stepper_type.");

	return in;
}

} // namespace num
} // namespace noma
