// Copyright (c) 2016-2017 Matthias Noack <ma.noack.pr@gmail.com>
//
// See accompanying file LICENSE and README for further information.

#include "noma/num/butcher_tableau.hpp"

namespace noma {
namespace num {

const butcher_tableau& get_butcher_tableau(const rk_method_t rkm)
{
	switch (rkm) {
		case rk_method_t::euler:
			return euler_tableau;
		case rk_method_t::midpoint:
			return midpoint_tableau;
		case rk_method_t::rk4:
			return rk4_tableau;
		case rk_method_t::fehlberg54:
			return fehlberg54_tableau;
		case rk_method_t::dopri54:
			return dopri54_tableau;
		case rk_method_t::cashkarp54:
			return cashkarp54_tableau;
		case rk_method_t::bosha32:
			return bosha32_tableau;
		default:
			throw std::runtime_error("get_butcher_tableau(): error: Unknown RK method");
	}
}

} // namespace num
} // namespace noma
