// Copyright (c) 2017 Matthias Noack <ma.noack.pr@gmail.com>
//
// See accompanying file LICENSE and README for further information.

#ifndef noma_num_make_stepper_hpp
#define noma_num_make_stepper_hpp

#include <noma/ocl/helper.hpp>

#include <memory>

#include "noma/num/meta_stepper.hpp"
#include "noma/num/polymorphic_stepper.hpp"
#include "noma/num/rk_stepper.hpp"
#include "noma/num/taylor_stepper.hpp"

namespace noma {
namespace num {

/**
 * @file
 * Collection of generator functions intended to create either a compile-time configured,
 * non-polymorphic stepper type, i.e. provided as template argument to a solver; or a
 * runtime-configured meta_stepper instance, that owns and manages a polymorphic_stepper
 * which is an interface to a polymorphic_stepper adapter wrapping a non-polymorphic
 * stepper implementation
 *
 *
 */

/**
 * Generator function for all stepper types but meta_stepper, which needs a
 * different ctor call.
 * Covers stepper ctor for embedded OpenCL kernel source.
 */
template<typename ODE, typename STEPPER>
typename std::enable_if<!std::is_same<STEPPER, num::meta_stepper>::value, STEPPER>::type // return type is not meta_stepper
make_stepper(const num::stepper_type_t& stepper_type, ocl::helper& ocl, const std::string& source_header, const std::string& ocl_compile_options, const ocl::nd_range& range, ODE& ode)
{
	// NOTE: stepper type is ignored
	return STEPPER(ocl, source_header, ocl_compile_options, range, ode);
}

/**
 * Generator function for meta_stepper, which needs a stepper_type_t to create
 * a certain stepper instance at runtime.
 * Covers stepper ctor for embedded OpenCL kernel source.
 */
template<typename ODE, typename STEPPER>
typename std::enable_if<std::is_same<STEPPER, num::meta_stepper>::value, STEPPER>::type // return type is meta_stepper
make_stepper(const num::stepper_type_t& stepper_type, ocl::helper& ocl, const std::string& source_header, const std::string& ocl_compile_options, const ocl::nd_range& range, ODE& ode)
{
	// NOTE: stepper type is passed
	return STEPPER(stepper_type, ocl, source_header, ocl_compile_options, range, ode);
}

/**
 * Generator function for all stepper types but meta_stepper, which needs a
 * different ctor call.
 * Covers stepper ctor for argument OpenCL kernel source.
 */
template<typename ODE, typename STEPPER>
typename std::enable_if<!std::is_same<STEPPER, num::meta_stepper>::value, STEPPER>::type // return type is not meta_stepper
make_stepper(const num::stepper_type_t& stepper_type, ocl::helper& ocl, const std::string& kernel_source, const std::string& kernel_name, const std::string& source_header, const std::string& ocl_compile_options, const ocl::nd_range& range, ODE& ode)
{
	// NOTE: stepper type is ignored
	return STEPPER(ocl, kernel_source, kernel_name, source_header, ocl_compile_options, range, ode);
}

/**
 * Generator function for meta_stepper, which needs a stepper_type_t to create
 * a certain stepper instance at runtime.
 * Covers stepper ctor for argument OpenCL kernel source.
 */
template<typename ODE, typename STEPPER>
typename std::enable_if<std::is_same<STEPPER, num::meta_stepper>::value, STEPPER>::type // return type is meta_stepper
make_stepper(const num::stepper_type_t& stepper_type, ocl::helper& ocl, const std::string& kernel_source, const std::string& kernel_name, const std::string& source_header, const std::string& ocl_compile_options, const ocl::nd_range& range, ODE& ode)
{
	// NOTE: stepper type is passed
	return STEPPER(stepper_type, ocl, kernel_source, kernel_name, source_header, ocl_compile_options, range, ode);
}

/**
 * Generator function for all stepper types but meta_stepper, which needs a
 * different ctor call.
 * Covers stepper ctor for file-loaded OpenCL kernel source.
 */
template<typename ODE, typename STEPPER>
typename std::enable_if<!std::is_same<STEPPER, num::meta_stepper>::value, STEPPER>::type // return type is not meta_stepper
make_stepper(const num::stepper_type_t& stepper_type, ocl::helper& ocl, const boost::filesystem::path& file_name, const std::string& kernel_name, const std::string& source_header, const std::string& ocl_compile_options, const ocl::nd_range& range, ODE& ode)
{
	// NOTE: stepper type is ignored
	return STEPPER(ocl, file_name, kernel_name, source_header, ocl_compile_options, range, ode);
}

/**
 * Generator function for meta_stepper, which needs a stepper_type_t to create
 * a certain stepper instance at runtime.
 * Covers stepper ctor for file-loaded OpenCL kernel source.
 */
template<typename ODE, typename STEPPER>
typename std::enable_if<std::is_same<STEPPER, num::meta_stepper>::value, STEPPER>::type // return type is meta_stepper
make_stepper(const num::stepper_type_t& stepper_type, ocl::helper& ocl, const boost::filesystem::path& file_name, const std::string& kernel_name, const std::string& source_header, const std::string& ocl_compile_options, const ocl::nd_range& range, ODE& ode)
{
	// NOTE: stepper type is passed
	return STEPPER(stepper_type, ocl, file_name, kernel_name, source_header, ocl_compile_options, range, ode);
}


/**
 * Generator function for OpenCL compile options, for all stepper types but meta_stepper.
 */
template<typename ODE, typename STEPPER, typename std::enable_if<!std::is_same<STEPPER, num::meta_stepper>::value, int>::type = 0> // type is not meta_stepper
void make_stepper_ode_compile_option(const num::stepper_type_t& stepper_type, std::ostream& os)
{
	// NOTE: stepper type is ignored, we call static member function of stepper
	STEPPER::ode_compile_options(os);
}

/**
 * Generator function for OpenCL compile options, for meta_stepper.
 */
template<typename ODE, typename STEPPER, typename std::enable_if<std::is_same<STEPPER, num::meta_stepper>::value, int>::type = 0> //  type is meta_stepper
void make_stepper_ode_compile_option(const num::stepper_type_t& stepper_type, std::ostream& os)
{
	// NOTE: this switch is the ugly but necessary way to get from the runtime stepper_type value, to an actual compile-time stepper type
	switch (stepper_type) {
		case stepper_type_t::rk_euler:
			stepper_type_to_type<ODE, stepper_type_t::rk_euler>::type::ode_compile_options(os);
			break;
		case stepper_type_t::rk_midpoint:
			stepper_type_to_type<ODE, stepper_type_t::rk_midpoint>::type::ode_compile_options(os);
			break;
		case stepper_type_t::rk_rk4:
			stepper_type_to_type<ODE, stepper_type_t::rk_rk4>::type::ode_compile_options(os);
			break;
		case stepper_type_t::rk_fehlberg54:
			stepper_type_to_type<ODE, stepper_type_t::rk_fehlberg54>::type::ode_compile_options(os);
			break;
		case stepper_type_t::rk_dopri54:
			stepper_type_to_type<ODE, stepper_type_t::rk_dopri54>::type::ode_compile_options(os);
			break;
		case stepper_type_t::rk_cashkarp54:
			stepper_type_to_type<ODE, stepper_type_t::rk_cashkarp54>::type::ode_compile_options(os);
			break;
		case stepper_type_t::rk_bosha32:
			stepper_type_to_type<ODE, stepper_type_t::rk_bosha32>::type::ode_compile_options(os);
			break;
		case stepper_type_t::taylor_1:
			stepper_type_to_type<ODE, stepper_type_t::taylor_1>::type::ode_compile_options(os);
			break;
		case stepper_type_t::taylor_2:
			stepper_type_to_type<ODE, stepper_type_t::taylor_2>::type::ode_compile_options(os);
			break;
		case stepper_type_t::taylor_3:
			stepper_type_to_type<ODE, stepper_type_t::taylor_3>::type::ode_compile_options(os);
			break;
		case stepper_type_t::taylor_4:
			stepper_type_to_type<ODE, stepper_type_t::taylor_4>::type::ode_compile_options(os);
			break;
		case stepper_type_t::taylor_5:
			stepper_type_to_type<ODE, stepper_type_t::taylor_5>::type::ode_compile_options(os);
			break;
		case stepper_type_t::taylor_6:
			stepper_type_to_type<ODE, stepper_type_t::taylor_6>::type::ode_compile_options(os);
			break;
		case stepper_type_t::taylor_7:
			stepper_type_to_type<ODE, stepper_type_t::taylor_7>::type::ode_compile_options(os);
			break;
		case stepper_type_t::taylor_8:
			stepper_type_to_type<ODE, stepper_type_t::taylor_8>::type::ode_compile_options(os);
			break;
		case stepper_type_t::taylor_9:
			stepper_type_to_type<ODE, stepper_type_t::taylor_9>::type::ode_compile_options(os);
			break;
		default:
			throw std::runtime_error("make_stepper_ode_compile_option(): error: called with unhandled stepper_type.");
	}
}

} // namespace num
} // namespace noma

#endif // noma_num_make_stepper_hpp
