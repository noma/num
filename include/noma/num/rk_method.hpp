// Copyright (c) 2016-2017 Matthias Noack <ma.noack.pr@gmail.com>
//
// See accompanying file LICENSE and README for further information.

#ifndef noma_num_rk_method_hpp
#define noma_num_rk_method_hpp

#include <iostream>
#include <map>

namespace noma {
namespace num {

enum class rk_method_t {
	euler,
	midpoint,
	rk4,
	fehlberg54,
	dopri54,
	cashkarp54,
	bosha32
};

const std::map<rk_method_t, std::string> rk_method_names {
	{ rk_method_t::euler, "euler" },
	{ rk_method_t::midpoint, "midpoint" },
	{ rk_method_t::rk4, "rk4" },
	{ rk_method_t::fehlberg54, "fehlberg54" },
	{ rk_method_t::dopri54, "dopri54" },
	{ rk_method_t::cashkarp54, "cashkarp54" },
	{ rk_method_t::bosha32, "bosha32" }
};

std::ostream& operator<<(std::ostream& out, const rk_method_t& m);
std::istream& operator>>(std::istream& in, rk_method_t& m);

} // namespace num
} // namespace noma

#endif // noma_num_rk_method_hpp
