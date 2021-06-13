/**
 * @defgroup BCG_Maths BCG Maths Module
 *
 * @brief commonly used extensions to the STL maths library
 *
 * @todo include multinomial
 *
 */

#ifndef BCG_MATHS_HPP
#define BCG_MATHS_HPP

// ========================================================================= //
// dependencies

#include <complex>

// ========================================================================== //

namespace BCG {
  //! @addtogroup BCG_Maths
  //! @{

  // ------------------------------------------------------------------------ //
  // globals

  //! an alias for \c std::complex<double>
  typedef std::complex<double> complex_d_t;

  //! @brief the numerical value of @f$\pi@f$
  constexpr double PI = std::atan(1.0) * 4;

  // ------------------------------------------------------------------------ //
  // procs

  /**
   * @brief returns a string representation of a std::complex<double> alias complex_d_t
   * @todo add more format params
   */

  template<class T>
  static inline const std::string complex_to_string(const std::complex<T> & z);

  /**
   * @brief computes the factorial of an integer n.
   *
   * @attention This is a template implementation that works with arbibrary
   *  types T. If the argument n is non-integral, this will go into an infinite
   *  loop
   *
   * * type unsigned <tt>long long int</tt> yields correct results up to n = 20
   *    and then goes into overload (returns zero)
   * * type \c double yields good results up to n = 22
   *    reaches computation limit after n = 170 and will yield <tt>nan:inf</tt>
   *    beyond (nothrow)
   * * type <tt>long double</tt> yields good results up to n = 25
   * * use at own risk with non-integral types, rounding errors will accumulate
   *
   * Can be used with GMP types and works just fine. However, GMP classes have
   * their proper methods for this.
   */
  template <typename T>
  static inline T factorial (T n);

  //! @}
}

// ========================================================================== //
// template implementations

#include "BCG/Maths.tpp"

#endif
