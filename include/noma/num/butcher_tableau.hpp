// Copyright (c) 2016-2017 Matthias Noack <ma.noack.pr@gmail.com>
//
// See accompanying file LICENSE and README for further information.

#ifndef noma_num_butcher_tableau_hpp
#define noma_num_butcher_tableau_hpp

#include <cassert>
#include <vector>

#include "noma/num/types.hpp"
#include "noma/num/rk_method.hpp"

namespace noma {
namespace num {

struct butcher_tableau
{
	using a_coeffs_t = std::vector<std::vector<real_t>>; // N x N matrix
	using b_coeffs_t = std::vector<real_t>; // row of N coeffs from B-part of tableau
	using c_coeffs_t = std::vector<real_t>; // column of N coeffs

	a_coeffs_t a;
	b_coeffs_t b;
	b_coeffs_t b_cmp; // an optional second b for integrated RK methods
	c_coeffs_t c;
};

/**
 * Returns a butcher_tablea for a given Runge-Kutta method (rk_method_t).
 */
const butcher_tableau& get_butcher_tableau(const rk_method_t rkm);

// Euler method, 1st order
// https://en.wikipedia.org/wiki/Runge%E2%80%93Kutta_methods#Examples
// https://en.wikipedia.org/wiki/Euler_method
const butcher_tableau euler_tableau	{
		// a coefficients
		{ { 0.0 } },
		// b coefficients
		{   1.0 },
		// b_cmp coefficients
		{ },
		// c coefficients
		{   0.0 }
	};

// midpoint method: 2nd order
// https://en.wikipedia.org/wiki/Runge%E2%80%93Kutta_methods#Examples
// https://en.wikipedia.org/wiki/Midpoint_method
const butcher_tableau midpoint_tableau	{
	// a coefficients
	{ { 0.0, 0.0 },
	  { 0.5, 0.0 } },
	// b coefficients
	{   0.0, 1.0 },
	// b_cmp coefficients
	{ },
	// c coefficients
	{   0.0, 0.5 }
};

// Classical Runge Kutte 4, 4th order
// https://en.wikipedia.org/wiki/Runge%E2%80%93Kutta_methods#Examples
const butcher_tableau rk4_tableau {
		// a coefficients
		{ {     0.0,     0.0,     0.0,     0.0 },
		  { 1.0/2.0,     0.0,     0.0,     0.0 },
		  {     0.0, 1.0/2.0,     0.0,     0.0 },
		  {     0.0,     0.0,     1.0,     0.0 } } ,
		// b coefficients
		{   1.0/6.0, 1.0/3.0, 1.0/3.0, 1.0/6.0 },
	    // b_cmp coefficients
		{ },
		// c coefficients
		{       0.0, 1.0/2.0, 1.0/2.0,     1.0 }
	};

// Runge Kutta Fehlberg, embedded 5th/4th order
// no FSAL, optimized for 4th order accuracy
// https://en.wikipedia.org/wiki/Runge%E2%80%93Kutta%E2%80%93Fehlberg_method
const butcher_tableau fehlberg54_tableau {
		// a coefficients, 6x6
		{ {           0.0,            0.0,            0.0,             0.0,        0.0,      0.0 },
		  {       1.0/4.0,            0.0,            0.0,             0.0,        0.0,      0.0 },
		  {      3.0/32.0,       9.0/32.0,            0.0,             0.0,        0.0,      0.0 },
		  { 1932.0/2197.0, -7200.0/2197.0,  7296.0/2197.0,             0.0,        0.0,      0.0 },
		  {   439.0/216.0,           -8.0,   3680.0/513.0,   -845.0/4104.0,        0.0,      0.0 },
		  {     -8.0/27.0,            2.0, -3544.0/2565.0,   1859.0/4104.0, -11.0/40.0,      0.0 } },
		// b coefficients, 5th order solution
		{      16.0/135.0,            0.0, 6656.0/12825.0, 28561.0/56430.0,  -9.0/50.0, 2.0/55.0 },
		// b_cmp coefficients, 4th order solution
		{      25.0/216.0,            0.0,  1408.0/2565.0,   2197.0/4104.0,   -1.0/5.0,      0.0 },
		// c coefficients
		{             0.0,        1.0/4.0,        3.0/8.0,       12.0/13.0,        1.0,  1.0/2.0 }
	};

// Dormand Prince, embedded 5th/4th order
// FSAL, optimized for 5th order accuracy
// https://en.wikipedia.org/wiki/Dormand%E2%80%93Prince_method
const butcher_tableau dopri54_tableau {
		// a coefficients, 7x7
		{ {            0.0,             0.0,            0.0,          0.0,               0.0,          0.0,      0.0 },
		  {        1.0/5.0,             0.0,            0.0,          0.0,               0.0,          0.0,      0.0 },
		  {       3.0/40.0,        9.0/40.0,            0.0,          0.0,               0.0,          0.0,      0.0 },
		  {      44.0/45.0,      -56.0/15.0,       32.0/9.0,          0.0,               0.0,          0.0,      0.0 },
		  { 19372.0/6561.0, -25360.0/2187.0, 64448.0/6561.0, -212.0/729.0,               0.0,          0.0,      0.0 },
		  {  9017.0/3168.0,     -355.0/33.0, 46732.0/5247.0,   49.0/176.0,   -5103.0/18656.0,          0.0,      0.0 },
		  {     35.0/384.0,             0.0,   500.0/1113.0,  125.0/192.0,    -2187.0/6784.0,    11.0/84.0,      0.0 } },
		// b coefficients, 5th order solution
		{       35.0/384.0,             0.0,   500.0/1113.0,  125.0/192.0,    -2187.0/6784.0,    11.0/84.0,      0.0 },
		// b_cmp coefficients, 4th order solution
		{   5179.0/57600.0,             0.0, 7571.0/16695.0,  393.0/640.0, -92097.0/339200.0, 187.0/2100.0, 1.0/40.0 },
		// c coefficients
		{              0.0,         1.0/5.0,       3.0/10.0,      4.0/5.0,           8.0/9.0,          1.0,      1.0 }
	};

// Cash Karp, embedded 5th/4th order
// https://en.wikipedia.org/wiki/Cash%E2%80%93Karp_method
const butcher_tableau cashkarp54_tableau {
		// a coefficients, 6x6
		{ {            0.0,         0.0,             0.0,              0.0,            0.0,          0.0 },
		  {        1.0/5.0,         0.0,             0.0,              0.0,            0.0,          0.0 },
		  {       3.0/40.0,    9.0/40.0,             0.0,              0.0,            0.0,          0.0 },
		  {       3.0/10.0,   -9.0/10.0,         6.0/5.0,              0.0,            0.0,          0.0 },
		  {     -11.0/54.0,     5.0/2.0,      -70.0/27.0,        35.0/27.0,            0.0,          0.0 },
		  { 1631.0/55296.0, 175.0/512.0,   575.0/13824.0, 44275.0/110592.0,   253.0/4096.0,          0.0 } },
		// b coefficients, 5th order solution
		{      37.0/378.0,          0.0,     250.0/621.0,      125.0/594.0,            0.0, 512.0/1771.0 },
		// b_cmp coefficients, 4th order solution
		{    2825.0/27648,          0.0, 18575.0/48384.0,   13525.0/55296.0, 277.0/14336.0,      1.0/4.0 },
		// c coefficients
		{             0.0,      1.0/5.0,        3.0/10.0,          3.0/5.0,            1.0,      7.0/8.0 }
	};

// Bogacki Shampine, embedded 3rd/2nd order
// FSAL
// https://en.wikipedia.org/wiki/Bogacki%E2%80%93Shampine_method
const butcher_tableau bosha32_tableau {
		// a coefficients
		{ {     0.0,     0.0,     0.0,     0.0 },
		  { 1.0/2.0,     0.0,     0.0,     0.0 },
		  {     0.0, 3.0/4.0,     0.0,     0.0 },
		  { 2.0/9.0, 1.0/3.0, 4.0/9.0,     0.0 } } ,
		// b coefficients
		{   2.0/9.0, 1.0/3.0, 4.0/9.0,     0.0 },
	    // b_cmp coefficients
		{  7.0/24.0, 1.0/4.0, 1.0/3.0, 1.0/8.0 },
		// c coefficients
		{       0.0, 1.0/2.0, 3.0/4.0,     1.0 }
	};

// TODO: exploit FSAL (First Same As Last) property (last k of step n is first k of next step n+1) of bosha32 and dopri54, maybe add flag to the tableau

} // namespace num
} // namespace noma

#endif // noma_num_butcher_tableau_hpp
