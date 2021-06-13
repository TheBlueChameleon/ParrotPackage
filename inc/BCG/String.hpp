/**
 * @defgroup BCG_String BCG Strings Module
 *
 * @brief commonly used extensions to the STL string library, mostly in the form
 *   of static inline functions
 */

#ifndef BCG_STRING_HPP
#define BCG_STRING_HPP

// ========================================================================= //
// dependencies

#include <stdexcept>

#include <string>
#include <vector>

// ========================================================================== //

namespace BCG {
  //! @addtogroup BCG_String
  //! @{

  // ------------------------------------------------------------------------ //
  // Trim String

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

  //! @}
}

// ========================================================================== //
// template implementations

#include "BCG/String.tpp"

#endif
