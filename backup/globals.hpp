#ifndef GLOBALS_HPP
#define GLOBALS_HPP

// ========================================================================== //
// dependencies

// STL
#include <stdexcept>

#include <iostream>
#include <fstream>

#include <vector>
#include <cctype>
#include <complex>

#include <cmath>
#include <cstdlib>
#include <ctime>

#include <algorithm> 
#include <numeric>
#include <functional>

#include <locale>

// ========================================================================== //
// namespace

namespace BCG {                                                                 // blue chameleon globals
  // ======================================================================== //
  // String utility

  /** @defgroup GlobalsString String Convenience Functions
   *  @{
   *
   * @brief provides convenience functions for use with the STL's std::string,
   *  mostly in the form of static functions
   */



  //! @} // group GlobalsString

  // ======================================================================== //
  // vector utility

  /** @defgroup GlobalsVector Vector Convenience Functions
   * @{
   *
   * @brief A collection of functions that are useful with the STL's std::vector<T>
   *
   * @todo extend
   */

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
  // show lists of lists Py-Style

  template<class T>
  static inline std::string vector_to_string(const std::vector<T> & list, bool brackets = true);

  template<class T>
  static inline std::string vecvec_to_string(const std::vector<std::vector<T>> & listlist);

  // ------------------------------------------------------------------------ //
  // vector distance

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

  // ======================================================================== //
  // File utilty

  /** @defgroup GlobalsFiles File Convenience Functions
   *  @{
   *
   * @brief TODO
   */
  std::string generateTimestamp();

  static inline std::fstream openThrow(const std::string & filename, std::ios_base::openmode mode = std::fstream::out);

  // ........................................................................ //

  static inline std::string  generateFileComments(const std::string & content);

  //! @}

}                                                                               // namespace BCG

// ========================================================================== //
// template implementations

#include "globals.tpp"

#endif
