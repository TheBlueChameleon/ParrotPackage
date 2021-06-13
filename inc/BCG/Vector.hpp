/**
 * @defgroup BCG_Vector BCG Vector Module
 *
 * @brief commonly used extensions to the STL vector library
 */

#ifndef BCG_VECTOR_HPP
#define BCG_VECTOR_HPP

// ========================================================================= //
// dependencies

#include <iostream>
#include <sstream>

#include <vector>

#include <cmath>

#include <numeric>

// ========================================================================== //

namespace BCG {
  //! @addtogroup BCG_Vector
  //! @{

  // ------------------------------------------------------------------------ //
  // convert

  //! @brief convert any iterable into a std::vector of according type
  template <class InputIt>
  inline std::vector<typename std::iterator_traits<InputIt>::value_type> to_vector (
    InputIt beg,
    InputIt end
  );

  // ------------------------------------------------------------------------ //
  // concatenate vectors

  template<class T>
  static inline std::vector<T> concatenate (const std::vector<T> & A, const std::vector<T> & B);

  template<class InputIt>
  static inline std::vector<typename std::iterator_traits<InputIt>::value_type> concatenate (
    InputIt begA, InputIt endA,
    InputIt begB, InputIt endB
  );

  template<class T>
  static inline void appendTo_vector (std::vector<T> & A, const std::vector<T> & B);

  // ------------------------------------------------------------------------ //
  // show lists and lists of lists Py-Style

  template<class InputIt>
  static inline std::string vector_to_string(InputIt beg, InputIt end, bool brackets = true);

  template<class T>
  static inline std::string vector_to_string(const std::vector<T> & list, bool brackets = true);

  template<class T>
  static inline std::string vecvec_to_string(const std::vector<std::vector<T>> & listlist);

  // ------------------------------------------------------------------------ //
  // vector distance

  //! @brief stuff
  //! @todo include optional norm function, take from findNearby, but use <functional>
  template<class T>
  double vector_distance(const std::vector<T> & A, const std::vector<T> & B);

  // ------------------------------------------------------------------------ //
  // find nearby

  template<class Iterator, class T>
  Iterator findNearby(Iterator begin, Iterator end,
                      const T & val,
                      double epsilon,
                      double absFunc (double) = std::abs
  );


  template<class Iterator, class T>
  int findNearbyIdx(Iterator begin, Iterator end,
                      const T & val,
                      double epsilon,
                      double absFunc (double) = std::abs
  );

  // ------------------------------------------------------------------------ //
  // generate useful vectors, NumPy-Style

  std::vector<double> linspace(const double start, const double end, const int N);

  //! @}
}

// ========================================================================== //
// template implementations

#include "BCG/Vector.tpp"

#endif
