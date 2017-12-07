// Copyright (c) 2015-2017 Matthias Noack <ma.noack.pr@gmail.com>
//
// See accompanying file LICENSE and README for further information.

#ifndef noma_noma_num_types_hpp
#define noma_noma_num_types_hpp

#include <cstdint> // fixed width integers
#include <complex>

#include <boost/format.hpp>

namespace noma {
namespace num {

#ifdef NUM_TYPES_SINGLE_PRECISION
#warning "Using single precision (float)"
using real_t         = float;
using long_real_t    = double;
//	using real_vec_t     = float_v;
#else
using real_t         = double;
using long_real_t    = long double;
//	using real_vec_t     = double_v;
#endif

using bool_t         = std::uint8_t; // needed for OpenCL memory layout interoperability
using int_t          = std::int32_t;
using uint_t         = std::uint32_t;
using complex_t      = std::complex<real_t>;
using long_complex_t = std::complex<long_real_t>;

// number of digits to print to get the bit-wisely equal real_t when reading back the data
int_t get_real_to_text_digits();

/* boost format instance for printing numbers, to
 * a) have a consistent format, and
 * b) not loose precision
 * use as a copy (to loose constness), outside of loops (for performance), e.g.
 *
 * auto format = real_format;
 * for (...) out << format % my_number;
 */
extern const boost::format int_format;
extern const boost::format real_format;
extern const boost::format complex_format;
// default delimiter character used for output
extern const char default_delimiter;

} // namespace num
} // namespace noma

#endif // noma_noma_num_types_hpp
