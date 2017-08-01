// Copyright (c) 2017 Matthias Noack <ma.noack.pr@gmail.com>
//
// See accompanying file LICENSE and README for further information.

#ifndef noma_num_stepper_type_hpp
#define noma_num_stepper_type_hpp

#include <iostream>
#include <map>

#include "noma/num/rk_stepper.hpp"
#include "noma/num/taylor_stepper.hpp"

namespace noma {
namespace num {

/**
 * NOTE: available stepper types to be used for solvers:
 *
 * Compile-time configured:
 * using stepper_t = num::rk_stepper<ODE_TYPE, num::rk_method_t::euler>;
 * using stepper_t = num::rk_stepper<ODE_TYPE, num::rk_method_t::midpoint>;
 * using stepper_t = num::rk_stepper<ODE_TYPE, num::rk_method_t::rk4>;
 * using stepper_t = num::rk_stepper<ODE_TYPE, num::rk_method_t::rk4, num::accumulate_method::separated>;
 * using stepper_t = num::rk_stepper<ODE_TYPE, num::rk_method_t::rk4, num::accumulate_method::integrated>;
 * using stepper_t = num::rk_stepper<ODE_TYPE, num::rk_method_t::rk4, num::accumulate_method::subdiagonal>;
 * using stepper_t = num::rk_stepper<ODE_TYPE, num::rk_method_t::fehlberg54>;
 * using stepper_t = num::rk_stepper<ODE_TYPE, num::rk_method_t::dopri54>;
 * using stepper_t = num::rk_stepper<ODE_TYPE, num::rk_method_t::cashkarp54>;
 * using stepper_t = num::rk_stepper<ODE_TYPE, num::rk_method_t::bosha32>;
 * using stepper_t = num::taylor_stepper<ODE_TYPE, 5>; // 5 can be any positive integer >=1
 *
 * Does not make much sense alone, but valid (with any wrapped stepper type), intended to be used with
 * polymorphic_stepper interface:
 * using stepper_t = num::polymorphic_stepper_adapter<num::taylor_stepper<ODE_TYPE, 5>>;
 * using stepper_t = num::polymorphic_stepper<num::polymorphic_stepper_adapter<num::taylor_stepper<ODE_TYPE, 5>>>
 *
 * Runtime configured:
 * using stepper_t = num::meta_stepper; // uses solver_stepper_t (see below), e.g. read from config
 */

/**
 * Parseable stepper type for runtime configurable stepper type, e.g. for configuration files.
 */
enum class stepper_type_t
{
	rk_euler = 0,
	rk_midpoint,
	rk_rk4,
	rk_fehlberg54,
	rk_dopri54,
	rk_cashkarp54,
	rk_bosha32,
	taylor_1,
	taylor_2,
	taylor_3,
	taylor_4,
	taylor_5,
	taylor_6,
	taylor_7,
	taylor_8,
	taylor_9
};

const std::map<stepper_type_t, std::string> stepper_type_names{
	{stepper_type_t::rk_euler,      "rk_euler"},
	{stepper_type_t::rk_midpoint,   "rk_midpoint"},
	{stepper_type_t::rk_rk4,        "rk_rk4"},
	{stepper_type_t::rk_fehlberg54, "rk_fehlberg54"},
	{stepper_type_t::rk_dopri54,    "rk_dopri54"},
	{stepper_type_t::rk_cashkarp54, "rk_cashkarp54"},
	{stepper_type_t::rk_bosha32,    "rk_bosha32"},
	{stepper_type_t::taylor_1, "taylor_1"},
	{stepper_type_t::taylor_2, "taylor_2"},
	{stepper_type_t::taylor_3, "taylor_3"},
	{stepper_type_t::taylor_4, "taylor_4"},
	{stepper_type_t::taylor_5, "taylor_5"},
	{stepper_type_t::taylor_6, "taylor_6"},
	{stepper_type_t::taylor_7, "taylor_7"},
	{stepper_type_t::taylor_8, "taylor_8"},
	{stepper_type_t::taylor_9, "taylor_9"}
};

std::ostream& operator<<(std::ostream& out, const stepper_type_t& t);
std::istream& operator>>(std::istream& in, stepper_type_t& t);

/**
 * Conversion from value to type.
 */
template<typename ODE, stepper_type_t STEPPER_TYPE>
struct stepper_type_to_type
{
	// undefined, compile error
};

template<typename ODE>
struct stepper_type_to_type<ODE, stepper_type_t::rk_euler>
{
	using type = rk_stepper<ODE, num::rk_method_t::euler, num::accumulate_method::separated>;
};

template<typename ODE>
struct stepper_type_to_type<ODE, stepper_type_t::rk_midpoint>
{
	using type = rk_stepper<ODE, num::rk_method_t::midpoint, num::accumulate_method::separated>;
};

template<typename ODE>
struct stepper_type_to_type<ODE, stepper_type_t::rk_rk4>
{
	using type = rk_stepper<ODE, num::rk_method_t::rk4, num::accumulate_method::separated>;
};

template<typename ODE>
struct stepper_type_to_type<ODE, stepper_type_t::rk_fehlberg54>
{
	using type = rk_stepper<ODE, num::rk_method_t::fehlberg54, num::accumulate_method::separated>;
};

template<typename ODE>
struct stepper_type_to_type<ODE, stepper_type_t::rk_dopri54>
{
	using type = rk_stepper<ODE, num::rk_method_t::dopri54, num::accumulate_method::separated>;
};

template<typename ODE>
struct stepper_type_to_type<ODE, stepper_type_t::rk_cashkarp54>
{
	using type = rk_stepper<ODE, num::rk_method_t::cashkarp54, num::accumulate_method::separated>;
};

template<typename ODE>
struct stepper_type_to_type<ODE, stepper_type_t::rk_bosha32>
{
	using type = rk_stepper<ODE, num::rk_method_t::bosha32, num::accumulate_method::separated>;
};


template<typename ODE>
struct stepper_type_to_type<ODE, stepper_type_t::taylor_1>
{
	using type = taylor_stepper<ODE, 1>;
};

template<typename ODE>
struct stepper_type_to_type<ODE, stepper_type_t::taylor_2>
{
	using type = taylor_stepper<ODE, 2>;
};

template<typename ODE>
struct stepper_type_to_type<ODE, stepper_type_t::taylor_3>
{
	using type = taylor_stepper<ODE, 3>;
};

template<typename ODE>
struct stepper_type_to_type<ODE, stepper_type_t::taylor_4>
{
	using type = taylor_stepper<ODE, 4>;
};

template<typename ODE>
struct stepper_type_to_type<ODE, stepper_type_t::taylor_5>
{
	using type = taylor_stepper<ODE, 5>;
};

template<typename ODE>
struct stepper_type_to_type<ODE, stepper_type_t::taylor_6>
{
	using type = taylor_stepper<ODE, 6>;
};

template<typename ODE>
struct stepper_type_to_type<ODE, stepper_type_t::taylor_7>
{
	using type = taylor_stepper<ODE, 7>;
};

template<typename ODE>
struct stepper_type_to_type<ODE, stepper_type_t::taylor_8>
{
	using type = taylor_stepper<ODE, 8>;
};

template<typename ODE>
struct stepper_type_to_type<ODE, stepper_type_t::taylor_9>
{
	using type = taylor_stepper<ODE, 9>;
};


} // namespace num
} // namespace noma

#endif // noma_num_stepper_type_hpp
