// Copyright (c) 2017 Matthias Noack <ma.noack.pr@gmail.com>
//
// See accompanying file LICENSE and README for further information.

#ifndef noma_num_taylor_stepper_hpp
#define noma_num_taylor_stepper_hpp

#include <cassert>

#include <noma/ocl/helper.hpp>

namespace noma {
namespace num {

/**
 * This class template performs a single integration step by computing the
 * Taylor series up to ORDER.
 *
 * WARNING: This is mathematically correct iff repeatedly applying the ODE_T
 * implementation, i.e. the first derivative, for n times yields the n-th
 * derivative.
 *
 * ORDER many evaluations of ODE_T are performed.
 * Independant of ORDER, two temporary state buffers are required. This makes
 * it more memory efficient than higher order Runge-Kutta methods.
 *
 * NOTE: only accumulate_method::integrated is implemented,
 * as accumulate_method::separated wouldn't deliver memory efficiency.
 */
template<typename ODE_T, size_t ORDER>
class taylor_stepper : public ocl::kernel_wrapper // NOTE: this class does not wrap a kernel, but needs to provide the same interface/concept, trying to run() it will throw
{
public:
	using ode_type = ODE_T;

	static constexpr accumulate_method acc_method = accumulate_method::integrated;

	taylor_stepper(ocl::helper& ocl, ODE_T& ode);

	// to fullfill the same 'concept' as rk_stepper.hpp, even though this stepper does not have its own OpenCL kernel
	taylor_stepper(ocl::helper& ocl, const std::string& source_header, const std::string& ocl_compile_options, const ocl::nd_range& range, ODE_T& ode)
		: taylor_stepper(ocl, ode) { };
	taylor_stepper(ocl::helper& ocl, const boost::filesystem::path& file_name, const std::string& kernel_name,
	               const std::string& source_header, const std::string& ocl_compile_options, const ocl::nd_range& range, ODE_T& ode)
		: taylor_stepper(ocl, ode) { };

	real_t step(real_t time, real_t step_size, cl::Buffer& d_mem_in, cl::Buffer& d_mem_out);

	// generate OpenCL compile options for ODE implementation
	static void ode_compile_options(std::ostream& os); // NOTE: needs to be static, as this is needed for ODE construction, which typically happens before stepper construction

private:
	void initialise();

	ODE_T& ode_;

	// OpenCL buffers
	cl::Buffer tmp_buffer_a_;
	cl::Buffer tmp_buffer_b_;
};

template<typename ODE_T, size_t ORDER>
taylor_stepper<ODE_T, ORDER>::taylor_stepper(ocl::helper& ocl, ODE_T& ode)
	: kernel_wrapper(ocl), ode_(ode) // NOTE: dummy initialisation of kernel_wrapper
{
	tmp_buffer_a_ = ocl_.create_buffer(CL_MEM_READ_WRITE, ode.buffer_size_byte(), nullptr);
	tmp_buffer_b_ = ocl_.create_buffer(CL_MEM_READ_WRITE, ode.buffer_size_byte(), nullptr);
}

template<typename ODE_T, size_t ORDER>
void taylor_stepper<ODE_T, ORDER>::ode_compile_options(std::ostream& os)
{
	os << "#define NOMA_NUM_ODE_ACCUMULATE" << "\n";
}

template<typename ODE_T, size_t ORDER>
real_t taylor_stepper<ODE_T, ORDER>::step(real_t time, real_t step_size, cl::Buffer& d_mem_in, cl::Buffer& d_mem_out)
{

	// compute first term of taylor series, k1
	// h = step_size
	// k1 = f(t_n, y_n), t_n not relevant, implicit via y_n = y(t_n)

	// NOTE:
	// we compute terms of the Taylor series here
	// y_(n+1) = y_n + sum(m) (h^m)/(m!)* m-th derivative
	// y_(n+1) = y_n + h^1/1! * y'(t) + h^2/2! * y''(t) + h^3/3! * y'''(t) + ...
	// with: y^(n)(t) = y'y^(n-1)(t)

	auto read_buffer  = [&](size_t i) -> cl::Buffer& { return (i % 2) == 1 ? tmp_buffer_a_ : tmp_buffer_b_; };
	auto write_buffer = [&](size_t i) -> cl::Buffer& { return (i % 2) == 1 ? tmp_buffer_b_ : tmp_buffer_a_; };

	// for first term
	real_t factorial = 1.0; // factorial of 0
	real_t step_size_power = step_size; // to the power of 0

	// initialise (true argument) d_mem_out with d_mem_in (y_n), i.e. the 0th Taylor series term
	// and accumulate 1st Taylor series term on top multiplied with acc_coff = step_size_power / factorial
	// read from d_mem_in, the first time, write into temporary write buffer, accumulate to d_mem_out, true means initialise accumulation buffer with d_mem_in
	// ode_.solve(time, step_size, d_mem_in, write_buffer(1), d_mem_out, 1.0 / factorial, true); // NOTE: this line is needed if result is multiplied with time_step within ode
	ode_.solve(time, 1.0, d_mem_in, write_buffer(1), d_mem_out, step_size_power / factorial, true);

	// compute and accumulate k_2 to k_n
	for (size_t i = 2; i <= ORDER; ++i)
	{
		factorial *= static_cast<real_t>(i);
		step_size_power *= step_size;
		// reads from last write buffer, and computes the i-th Taylor series term, and accumulates it with coeff = step_size_power / factorial
		// read from temporary buffer, write into temporary write buffer, switches every time, accumulate to d_mem_out
		// ode_.solve(time, step_size, read_buffer(i), write_buffer(i), d_mem_out, 1.0 / factorial, false);  // NOTE: this line is needed if result is multiplied with time_step within ode
		ode_.solve(time, 1.0, read_buffer(i), write_buffer(i), d_mem_out, step_size_power / factorial, false);
	}

	// TODO(adaptive time step): maybe extend series ORDER+1 and compare
	return 0.0;
}

} // namespace num
} // namespace noma

#endif // noma_num_taylor_stepper_hpp
