// Copyright (c) 2017 Matthias Noack <ma.noack.pr@gmail.com>
//
// See accompanying file LICENSE and README for further information.

#ifndef noma_num_polymorphic_stepper_hpp
#define noma_num_polymorphic_stepper_hpp

#include <memory>
#include <noma/ocl/helper.hpp>

#include "noma/num/rk_stepper.hpp"
#include "noma/num/stepper_type.hpp"
#include "noma/num/taylor_stepper.hpp"

namespace noma {
namespace num {

/**
 * Polymorphic, purely abstract interface for stepper.
 */
class polymorphic_stepper {
public:
	// public stepper interface
	virtual real_t step(real_t time, real_t step_size, cl::Buffer& d_mem_in, cl::Buffer& d_mem_out) = 0;

	// public kernel wrapper interface (expected super class of stepper)
	virtual ocl::helper& ocl_helper() = 0;
	virtual bool uses_kernel_file() const = 0;
	virtual const std::string& kernel_file_name() const = 0;
	virtual const noma::bmt::statistics& kernel_stats() const = 0;
};


/**
 * Adapter class to make non-polymorphic stepper classes polymorphic, i.e. runtime-exchangeable.
 */
template<typename STEPPER>
class polymorphic_stepper_adapter : private STEPPER, public polymorphic_stepper
{
public:
	polymorphic_stepper_adapter(ocl::helper& ocl, const std::string& source_header, const std::string& ocl_compile_options, const ocl::nd_range& range, typename STEPPER::ode_type& ode)
		: STEPPER(ocl, source_header, ocl_compile_options, range, ode)
	{ }

	polymorphic_stepper_adapter(ocl::helper& ocl, const boost::filesystem::path& file_name, const std::string& kernel_name,
	                            const std::string& source_header, const std::string& ocl_compile_options, const ocl::nd_range& range, typename STEPPER::ode_type& ode)
		: STEPPER(ocl, file_name, kernel_name, source_header, ocl_compile_options, range, ode)
	{ }

	polymorphic_stepper_adapter()
	{

	}

	virtual real_t step(real_t time, real_t step_size, cl::Buffer& d_mem_in, cl::Buffer& d_mem_out)
	{
		return STEPPER::step(time, step_size, d_mem_in, d_mem_out);
	}

	virtual ocl::helper& ocl_helper()
	{
		return STEPPER::ocl_helper();
	}

	virtual bool uses_kernel_file() const
	{
		return STEPPER::uses_kernel_file();
	}

	virtual const std::string& kernel_file_name() const
	{
		return STEPPER::kernel_file_name();
	}

	virtual const noma::bmt::statistics& kernel_stats() const
	{
		return STEPPER::kernel_stats();
	}
};

/**
 * Generator function to create a unique pointer of the polymorphic interface
 * type polymorphic_stepper for a given (runtime-configured) stepper_type_t
 * value.
 *
 * ODE is the ODE type for the create stepper
 * Args are forwarde to the stepper constructor
 */
template<typename ODE, typename... Args>
std::unique_ptr<polymorphic_stepper> make_unique_polymorphic_stepper(const stepper_type_t& stepper_type, Args&&... args)
{
	// NOTE: this switch is the ugly but necessary way to get from the runtime stepper_type value, to an actual compile-time stepper type
	switch (stepper_type) {
		case stepper_type_t::rk_euler:
			return std::unique_ptr<polymorphic_stepper>(new polymorphic_stepper_adapter<typename stepper_type_to_type<ODE, stepper_type_t::rk_euler>::type>(std::forward<Args>(args)...));
		case stepper_type_t::rk_midpoint:
			return std::unique_ptr<polymorphic_stepper>(new polymorphic_stepper_adapter<typename stepper_type_to_type<ODE, stepper_type_t::rk_midpoint>::type>(std::forward<Args>(args)...));
		case stepper_type_t::rk_rk4:
			return std::unique_ptr<polymorphic_stepper>(new polymorphic_stepper_adapter<typename stepper_type_to_type<ODE, stepper_type_t::rk_rk4>::type>(std::forward<Args>(args)...));
		case stepper_type_t::rk_fehlberg54:
			return std::unique_ptr<polymorphic_stepper>(new polymorphic_stepper_adapter<typename stepper_type_to_type<ODE, stepper_type_t::rk_fehlberg54>::type>(std::forward<Args>(args)...));
		case stepper_type_t::rk_dopri54:
			return std::unique_ptr<polymorphic_stepper>(new polymorphic_stepper_adapter<typename stepper_type_to_type<ODE, stepper_type_t::rk_dopri54>::type>(std::forward<Args>(args)...));
		case stepper_type_t::rk_cashkarp54:
			return std::unique_ptr<polymorphic_stepper>(new polymorphic_stepper_adapter<typename stepper_type_to_type<ODE, stepper_type_t::rk_cashkarp54>::type>(std::forward<Args>(args)...));
		case stepper_type_t::rk_bosha32:
			return std::unique_ptr<polymorphic_stepper>(new polymorphic_stepper_adapter<typename stepper_type_to_type<ODE, stepper_type_t::rk_bosha32>::type>(std::forward<Args>(args)...));
		case stepper_type_t::taylor_1:
			return std::unique_ptr<polymorphic_stepper>(new polymorphic_stepper_adapter<typename stepper_type_to_type<ODE, stepper_type_t::taylor_1>::type>(std::forward<Args>(args)...));
		case stepper_type_t::taylor_2:
			return std::unique_ptr<polymorphic_stepper>(new polymorphic_stepper_adapter<typename stepper_type_to_type<ODE, stepper_type_t::taylor_2>::type>(std::forward<Args>(args)...));
		case stepper_type_t::taylor_3:
			return std::unique_ptr<polymorphic_stepper>(new polymorphic_stepper_adapter<typename stepper_type_to_type<ODE, stepper_type_t::taylor_3>::type>(std::forward<Args>(args)...));
		case stepper_type_t::taylor_4:
			return std::unique_ptr<polymorphic_stepper>(new polymorphic_stepper_adapter<typename stepper_type_to_type<ODE, stepper_type_t::taylor_4>::type>(std::forward<Args>(args)...));
		case stepper_type_t::taylor_5:
			return std::unique_ptr<polymorphic_stepper>(new polymorphic_stepper_adapter<typename stepper_type_to_type<ODE, stepper_type_t::taylor_5>::type>(std::forward<Args>(args)...));
		case stepper_type_t::taylor_6:
			return std::unique_ptr<polymorphic_stepper>(new polymorphic_stepper_adapter<typename stepper_type_to_type<ODE, stepper_type_t::taylor_6>::type>(std::forward<Args>(args)...));
		case stepper_type_t::taylor_7:
			return std::unique_ptr<polymorphic_stepper>(new polymorphic_stepper_adapter<typename stepper_type_to_type<ODE, stepper_type_t::taylor_7>::type>(std::forward<Args>(args)...));
		case stepper_type_t::taylor_8:
			return std::unique_ptr<polymorphic_stepper>(new polymorphic_stepper_adapter<typename stepper_type_to_type<ODE, stepper_type_t::taylor_8>::type>(std::forward<Args>(args)...));
		case stepper_type_t::taylor_9:
			return std::unique_ptr<polymorphic_stepper>(new polymorphic_stepper_adapter<typename stepper_type_to_type<ODE, stepper_type_t::taylor_9>::type>(std::forward<Args>(args)...));
		default:
			throw std::runtime_error("make_unique_polymorphic_stepper(): error: called with unhandled stepper_type.");
	}
}

} // namespace num
} // namespace noma

#endif // noma_num_polymorphic_stepper_hpp
