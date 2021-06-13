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
  // type alias

  /**
   * @defgroup GlobalSymbols Global Symbols
   *  @{
   *
   * @brief Global definitions used by different parts of the module
   */



  // ======================================================================== //
  // generic

  // ------------------------------------------------------------------------ //
  // globals


  //! @}                                                                        end group GlobalSymbols

  // ======================================================================== //
  // console output convenience

  /**
   * @defgroup GlobalsConsole Console Convenience Functions
   * @{
   *
   * @brief Features that allow for convenient output on terminals
   * @todo Revise these features to support redirecting.
   */





  //! @} // group GlobalsConsole

  // ======================================================================== //
  // String utility

  /** @defgroup GlobalsString String Convenience Functions
   *  @{
   *
   * @brief provides convenience functions for use with the STL's std::string,
   *  mostly in the form of static functions
   */

  // ------------------------------------------------------------------------ //
  // Trim String
  // Source:https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring

  //! @brief trim whitespaces from the start of the string, in place
  static inline void ltrim(std::string &s);

  //! @brief trim whitespaces from the end of the string, in place
  static inline void rtrim(std::string &s);

  //! @brief trim whitespaces from the stard and end of the string, in place
  static inline void trim(std::string &s);

  //! @brief return a copy where whitespaces from the start of the string are trimmed
  static inline std::string ltrim_copy(std::string s);

  //! @brief return a copy where whitespaces from the end of the string are trimmed
  static inline std::string rtrim_copy(std::string s);

  //! @brief return a copy where whitespaces from the start and end of the string are trimmed
  static inline std::string trim_copy(std::string s);

  // ........................................................................ //
  //! @brief remove all whitespaces from a string, in place
  static inline void fullTrim(std::string &s);

  //! @brief return a copy of the string where all whitespaces are removed
  static inline std::string fullTrim_copy(std::string s);

  // ------------------------------------------------------------------------ //
  // case conversion

  //! @brief convert the entire string to uppercase characters
  static inline void     to_uppercase(std::string & s);

  //! @brief return a copy of the string where all characters are converted to uppercase
  static inline std::string uppercase(std::string   s);

  // ------------------------------------------------------------------------ //
  // split string

  /**
   * @brief split the string into a list of strings
   *
   * The separator character will be removed from the resulting list
   *
   * @param s the string to split into a list of strings
   * @param separator the character that indicates where to split the string
   */
  std::vector<std::string> splitString(const std::string & s, const char separator = ',');

  // ........................................................................ //
  // wildcard matching

  /**
   * @brief check whether a string is matched by a pattern
   *
   * @param pattern a string to match against. All characters other than \c ? and \c * are matched verbatim.
   *  * \c ? will be interpreted as a wildcard for an arbitrary \e single character
   *  * \c * will be interpreted as a wildcard for an arbitrary \e sequence of characters
   * @param toMatch is the string being analyzed.
   */
  bool wildcardmatch(const char        * pattern, const char        * toMatch);

  /**
   * @brief check whether a string is matched by a pattern
   *  see wildcardmatch(const char *, const char *) for details
   */
  bool wildcardmatch(const std::string & pattern, const std::string & toMatch);

  // ------------------------------------------------------------------------ //
  // complex



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

  // ======================================================================== //
  // misc

  /** @defgroup GlobalsMisc Misc Convenience Functions
   *  @{
   *
   * @brief TODO
   */



  // ........................................................................ //

  //! @}


}                                                                               // namespace BCG

// ========================================================================== //
// template implementations

#include "globals.tpp"

#endif
