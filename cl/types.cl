// Copyright (c) 2015-2017 Matthias Noack <ma.noack.pr@gmail.com>
//
// See accompanying file LICENSE and README for further information.

#if __OPENCL_VERSION__ <= CL_VERSION_1_1
	#pragma OPENCL EXTENSION cl_khr_fp64 : enable
#endif

typedef uchar bool_t; // bool is not allowed for kernel arguments
typedef int int_t;
typedef uint uint_t;

#ifndef VEC_LENGTH
	#define VEC_LENGTH 8
#endif

#ifdef SINGLE_PRECISION
	#define FLOATVEC_HELPER(n) float ## n
	#define FLOATVEC(n) FLOATVEC_HELPER(n)
	typedef FLOATVEC(VEC_LENGTH) real_vec_t;
	typedef FLOATVEC(2) real_2_t;
	typedef float real_t;
	typedef float2 complex_t;
#else
	#define DOUBLEVEC_HELPER(n) double ## n
	#define DOUBLEVEC(n) DOUBLEVEC_HELPER(n)
	typedef DOUBLEVEC(VEC_LENGTH) real_vec_t;
	typedef DOUBLEVEC(2) real_2_t;
	typedef double real_t;
	typedef double2 complex_t;
#endif

inline complex_t conj(complex_t a)
{
	return (complex_t)(a.x, -a.y);
}

inline real_t real(complex_t a)
{
	return (real_t)a.x;
}

inline complex_t cmult(complex_t a, complex_t b)
{
	return (complex_t)(a.x*b.x - a.y*b.y, a.x*b.y + a.y*b.x);
}

inline complex_t cadd(complex_t a, complex_t b)
{
	return (complex_t)(a.x+b.x,a.y+b.y);
}

inline complex_t cdiv(complex_t a, complex_t b)
{
	return (complex_t)((a.x*b.x + a.y*b.y)/(b.x*b.x + b.y*b.y), (a.y*b.x - a.x*b.y)/(b.x*b.x + b.y*b.y));
}

inline real_t cnorm(complex_t a) // TODO: decide whether to apply sqrt(..) here
{
	return a.x * a.x + a.y * a.y;
}

