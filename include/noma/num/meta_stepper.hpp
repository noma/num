// Copyright (c) 2017 Matthias Noack <ma.noack.pr@gmail.com>
//
// See accompanying file LICENSE and README for further information.

#ifndef noma_num_meta_stepper_hpp
#define noma_num_meta_stepper_hpp

#include <memory>
#include <noma/ocl/helper.hpp>

#include "noma/num/polymorphic_stepper.hpp"

namespace noma {
namespace num {

/**
 * Wrapper class that manages creation and ownership of a polymorphic_stepper(_adapter).
 *
 * Implements the same interface as polymorphic_stepper, and forwards calls.
 * Uses make_unique_polymorphic_stepper() for creation.
 */
class meta_stepper {
public:

	template<typename ODE>
	meta_stepper(const stepper_type_t& stepper_type, ocl::helper& ocl, const std::string& source_header, const std::string& ocl_compile_options, const ocl::nd_range& range, ODE& ode)
		: poly_stepper_(make_unique_polymorphic_stepper<ODE>(stepper_type, ocl, source_header, ocl_compile_options, range, ode)) // NOTE: all but first are ctor arguments
	{ }

	template<typename ODE>
	meta_stepper(const stepper_type_t& stepper_type, ocl::helper& ocl, const std::string& kernel_source, const std::string& kernel_name,
	             const std::string& source_header, const std::string& ocl_compile_options, const ocl::nd_range& range, ODE& ode)
		: poly_stepper_(make_unique_polymorphic_stepper<ODE>(stepper_type, ocl, kernel_source, kernel_name, source_header, ocl_compile_options, range, ode)) // NOTE: all but first are ctor arguments
	{ }

	template<typename ODE>
	meta_stepper(const stepper_type_t& stepper_type, ocl::helper& ocl, const boost::filesystem::path& file_name, const std::string& kernel_name,
	             const std::string& source_header, const std::string& ocl_compile_options, const ocl::nd_range& range, ODE& ode)
		: poly_stepper_(make_unique_polymorphic_stepper<ODE>(stepper_type, ocl, file_name, kernel_name, source_header, ocl_compile_options, range, ode)) // NOTE: all but first are ctor arguments
	{ }


	// public stepper interface
	real_t step(real_t time, real_t step_size, cl::Buffer& d_mem_in, cl::Buffer& d_mem_out)
	{
		return poly_stepper_->step(time, step_size, d_mem_in, d_mem_out);
	}

	// public kernel wrapper interface (expected super class of stepper)
	ocl::helper& ocl_helper()
	{
		return poly_stepper_->ocl_helper();
	}

	bool uses_kernel_file() const
	{
		return poly_stepper_->uses_kernel_file();
	}

	const std::string& kernel_file_name() const
	{
		return poly_stepper_->kernel_file_name();
	}

	const noma::bmt::statistics& kernel_stats() const
	{
		return poly_stepper_->kernel_stats();
	}

private:
	std::unique_ptr<polymorphic_stepper> poly_stepper_;
};

} // namespace num
} // namespace noma

#endif // noma_num_meta_stepper_hpp
