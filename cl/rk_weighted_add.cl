// Copyright (c) 2015-2017 Matthias Noack <ma.noack.pr@gmail.com>
//
// See accompanying file LICENSE and README for further information.

#include "types.cl"

// expected defines: NUM_MATRICES, NUM_STATES

/**
 * computes the weighted sum:
 * out = y_n + h * sum(i)(coeffs(i) * k(i))
 * used in RK solver at two points:
 * k(i) = f(t + ..., HERE)
 * y(n+1) = HERE
 */
__kernel void rk_weighted_add(
	const int    n,                         // number of terms to add, i.e. coefficients to use
	const real_t h,                         // step width
	__global       real_t* restrict out,    // output buffer (accumulated on)
	__global const real_t* restrict y_n,    // input buffers 
	real_t c1,
	__global const real_t* restrict k1,     // ks as in Runge Kutta methods
	real_t c2,
	__global const real_t* restrict k2,
	real_t c3,
	__global const real_t* restrict k3,
	real_t c4,
	__global const real_t* restrict k4, 
	real_t c5,
	__global const real_t* restrict k5, 
	real_t c6,
	__global const real_t* restrict k6,
	real_t c7,
	__global const real_t* restrict k7      // TODO: add more if needed

)
{
	// sigma matrix id processed by this work item
	#define sigma_id (get_global_id(1) * get_global_size(0) + get_global_id(0))
	#define sigma_real(i, j) (2 * (sigma_id * NUM_STATES * NUM_STATES + (i) * NUM_STATES + (j)))
	#define sigma_imag(i, j) (2 * (sigma_id * NUM_STATES * NUM_STATES + (i) * NUM_STATES + (j)) + 1)

	// skip padded work-items
	if (sigma_id >= NUM_MATRICES)
		return;

	const real_t c[] = { c1, c2, c3, c4, c5, c6, c7 }; // TODO: add more if needed
	__global const real_t* restrict k[] = { k1, k2, k3, k4, k5, k6, k7 }; // TODO: add more if needed

	// process matrix elements
	for (int i = 0; i < NUM_STATES; ++i) // row
	{
		for (int j = 0; j < NUM_STATES; ++j) // column
		{
//			out[sigma_real(i,j)] = y_n[sigma_real(i,j)];
//			out[sigma_imag(i,j)] = y_n[sigma_imag(i,j)];
			out[sigma_real(i,j)] = 0.0;
			out[sigma_imag(i,j)] = 0.0;

			// iterate through coefficients and ks and accumulate them on out
			for (int l = 0; l < n; ++l)
			{
//				out[sigma_real(i,j)] += h * c[l] * k[l][sigma_real(i,j)];
//				out[sigma_imag(i,j)] += h * c[l] * k[l][sigma_imag(i,j)];
				if (c[l] != 0.0) 
				{
					out[sigma_real(i,j)] += c[l] * k[l][sigma_real(i,j)];
					out[sigma_imag(i,j)] += c[l] * k[l][sigma_imag(i,j)];
				}
			}
			out[sigma_real(i,j)] *= h;
			out[sigma_imag(i,j)] *= h;
			out[sigma_real(i,j)] += y_n[sigma_real(i,j)];
			out[sigma_imag(i,j)] += y_n[sigma_imag(i,j)];
		}
	}
}
