/**
 * @defgroup BCG_Vector BCG Vector Module
 *
 * @brief commonly used extensions to the STL vector library
 */

#ifndef BCG_VECTOR_HPP
#define BCG_VECTOR_HPP

// ========================================================================= //
// dependencies

#include <stdexcept>
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

  /**
   * @brief convert any iterable into a std::vector of according type
   * @param beg iterator to the begin of the container
   * @param end iterator to the end of the container
   */
  template <class InputIt>
  inline std::vector<typename std::iterator_traits<InputIt>::value_type> to_vector (
    InputIt beg,
    InputIt end
  );

  // ------------------------------------------------------------------------ //
  // concatenate vectors

  /**
   * @brief create an std::vector<T> containing the direct sum of any two
   *  compatible STL iterables.
   *
   * @b Example: <br>
   * Let there be STL containers \c A and \c B, with the following generic
   * content:
   * @code
   * A = [1, 2, 3]
   * B = [4, 5]
   * @endcode
   * Then concatenating them will yield a std::vector<T> with the content
   * @code
   * [1, 2, 3, 4, 5]
   * @endcode
   *
   * @param begA iterator to the begin of the first container
   * @param endA iterator to the end of the first container
   * @param begB iterator to the begin of the second container
   * @param endB iterator to the end of the second container
   */
  template<class InputIt>
  static inline std::vector<typename std::iterator_traits<InputIt>::value_type> concatenate (
    InputIt begA, InputIt endA,
    InputIt begB, InputIt endB
  );

  /**
   * @brief convenience forwarder to the generic version of concatenate(),
   *  specific for std::vector<T>'s
   */
  template<class T>
  static inline std::vector<T> concatenate (const std::vector<T> & A, const std::vector<T> & B);

  /**
   * @brief alters \c A by appending the elements of \c B
   *
   * @b Example: <br>
   * Let there be STL containers \c A and \c B, with the following generic
   * content:
   * @code
   * A = [1, 2, 3]
   * B = [4, 5]
   * @endcode
   * Then \c A will be changed to hold the content:
   * @code
   * [1, 2, 3, 4, 5]
   * @endcode
   * while \c B remains as is.
   *
   * @param A the vector to be changed by appending elements
   * @param B the source vector for the data to be appended
   */
  template<class T>
  static inline void appendTo_vector (std::vector<T> & A, const std::vector<T> & B);

  // ------------------------------------------------------------------------ //
  // show lists and lists of lists Py-Style

  /**
   * @brief type generic rendition of a STL iterable as an std::string
   *
   * produces a string in the like of
   * <tt>[element1, element2, ...]</tt>
   *
   * @param beg iterator to the begin of the container
   * @param end iterator to the end of the container
   * @param brackets flag, indicating whether or not the produced string should
   *  be enclosed by [brackets]
   *
   * @attention this assumes that the streaming operator << is defined on the
   *  iterator's underlying value_type.
   */
  template<class InputIt>
  static inline std::string vector_to_string(InputIt beg, InputIt end, bool brackets = true);

  /**
   * @brief convenience forwarder to the generic version of vector_to_string(),
   *  specific for std::vector<T>'s
   */
  template<class T>
  static inline std::string vector_to_string(const std::vector<T> & list, bool brackets = true);

  /**
   * @brief renders a two-level vector<vector<T>> into a \c std::string.
   *  Reqires \c T to implement the streaming operator <<.
   */
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

  /**
   * @brief returns an iterator to the first object within \c epsilon of a
   *  searchword.
   *
   * @param begin iterator to the first element in the container to look through
   * @param end   iterator to the last element in the container to look through
   * @param val value to look for
   * @param epsilon maximum distance between container element and \c val to
   *  accept as equivalent
   * @param difffunc function used to compute the difference between the
   *  container element and \c val
   * @param absfunc function used to project the difference between \c val and
   *  the container element onto the positive reals
   *
   */
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
