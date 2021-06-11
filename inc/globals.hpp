/**
 * @defgroup Globals Globals Module
 *
 * @{
 * @brief A collection of useful general purpose functions
 *
 * This Module collects a whole bunch of useful functions. They are grouped into
 * groups pertaining to the tasks they help achieve. See the following pages for
 * details on their effects:
 *
 * - @subpage GlobalsConsole
 * - @subpage GlobalsString
 * - @subpage GlobalsVector
 * - @subpage GlobalsFiles
 * - @subpage GlobalsMisc
 * - @subpage GlobalSymbols
 *
 * @todo finish!
 * @}
 */

#ifndef GLOBALS_HPP
#define GLOBALS_HPP

// ========================================================================== //
// dependencies

// STL
#include <stdexcept>

#include <iostream>
#include <sstream>
#include <fstream>

#include <vector>
#include <cctype>
#include <complex>

#include <cmath>
#include <random>
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
  typedef std::complex<double> complex_d_t;

  // ======================================================================== //
  // generic

  // ------------------------------------------------------------------------ //
  // globals

  constexpr double PI = std::atan(1.0)*4;

  // ======================================================================== //
  // random number generator

  extern std::random_device RNG;
  extern std::uniform_real_distribution<> rand_phase_distribution;

  double get_randPhase();

  //! @}

  // ======================================================================== //
  // console output convenience

  /**
   * @defgroup GlobalsConsole Console Convenience Functions
   * @{
   *
   * @brief Features that allow for convenient output on terminals
   * @todo Revise these features to support redirecting.
   */

  /**
   * @brief Lists all the format specifiers compatible with common linux consoles.
   *  Use with a ConsoleColorsTriple
   */
  enum class ConsoleColors {
    FORE_BLACK,
    FORE_RED,
    FORE_GREEN,
    FORE_BROWN,
    FORE_BLUE,
    FORE_PURPLE,
    FORE_CYAN,
    FORE_DARK_GREY,
    FORE_NORMAL,
    FORE_BRIGHT_GREY,
    FORE_BRIGHT_RED,
    FORE_BRIGHT_GREEN,
    FORE_YELLOW,
    FORE_BRIGHT_BLUE,
    FORE_PINK,
    FORE_BRIGHT_CYAN,
    FORE_WHITE,

    BACK_BLACK,
    BACK_RED,
    BACK_GREEN,
    BACK_BROWN,
    BACK_BLUE,
    BACK_PURPLE,
    BACK_CYAN,
    BACK_DARK_GREY,
    BACK_NORMAL,
    BACK_BRIGHT_GREY,
    BACK_BRIGHT_RED,
    BACK_BRIGHT_GREEN,
    BACK_YELLOW,
    BACK_BRIGHT_BLUE,
    BACK_PINK,
    BACK_BRIGHT_CYAN,
    BACK_WHITE,

    SPC_NORMAL,
    SPC_UNDERLINE_ON,
    SPC_UNDERLINE_OFF,
    SPC_ITALICS_ON,
    SPC_ITALICS_OFF,
    SPC_BLINK_ON,
    SPC_BLINK_OFF,
    SPC_BOLD_ON,
    SPC_BOLD_OFF,
  };

  /**
   * @brief A specification for a console text format. Can be used with consoleSetcolor,
   *    utterWarning and coutHeadline
   *
   * While there are 2 times 16 colors defined (fore and background), not all
   * consoles support all of them. Be prepared to find the output to be modulo 8
   * (i.e. that there will be no bright versions of the colours).
   * Likewise, it is not guaranteed that the special formats (prefix SPC_) are
   * respected by your console.
   *
   * @param fore color attribute of the characters themselves. Must be one of the FORE_* values
   * @param back color attribute of the character background. Must be one of the BACK_* values
   * @param spc  special format attribute for the characters themselves. Must be one of the SPC_* values of ConsoleColors
   */
  struct ConsoleColorsTriple {
    ConsoleColors fore = ConsoleColors::FORE_WHITE;
    ConsoleColors back = ConsoleColors::BACK_BLACK;
    ConsoleColors spc  = ConsoleColors::SPC_NORMAL;
  };

  //! @brief clear the screen
  static inline void consoleClear()                   {std::cout << "\033[H\033[J";}

  /**
   * @brief places the cursor in (row, col)
   * @param row the row where the console cursor should be placed
   * @param col the column where the console cursor should be placed
   */
  static inline void consoleGotoRC(int row, int col)  {std::cout << "\033[" << row << ";" << col << "H";}

  /**
   * @brief changes the output format for subsequent outputs to stdout
   * @todo make the target generic
   */
  void consoleSetcolor (ConsoleColors code);

  /**
   * @brief puts a warning message on stderr
   * @todo make target generic
   */
  void utterWarning ( const std::string & text,
                      const std::string & headline = "Warning",
                      const int indentFirst   = 0,
                      const int indentHanging = 3,
                      const ConsoleColorsTriple & headlineColors = {ConsoleColors::FORE_BRIGHT_RED},
                      const ConsoleColorsTriple & textColors     = {ConsoleColors::FORE_NORMAL}
  );

  /**
   * @brief puts a headline on stdout
   * @todo make target generic
   * @todo include params for border characters
   */
  void coutHeadline ( const std::string & text,
                      const ConsoleColorsTriple & headlineColors = {ConsoleColors::FORE_BRIGHT_RED}
  );

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
  static inline void ltrim(std::string &s) {
      s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
          return !std::isspace(ch);
      }));
  }
  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .//
  //! @brief trim whitespaces from the end of the string, in place
  static inline void rtrim(std::string &s) {
      s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
          return !std::isspace(ch);
      }).base(), s.end());
  }
  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .//
  //! @brief trim whitespaces from the stard and end of the string, in place
  static inline void trim(std::string &s) {ltrim(s); rtrim(s);}
  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .//
  //! @brief return a copy where whitespaces from the start of the string are trimmed
  static inline std::string ltrim_copy(std::string s) {ltrim(s); return s;}
  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .//
  //! @brief return a copy where whitespaces from the end of the string are trimmed
  static inline std::string rtrim_copy(std::string s) {rtrim(s); return s;}
  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .//
  //! @brief return a copy where whitespaces from the start and end of the string are trimmed
  static inline std::string trim_copy(std::string s) {trim(s); return s;}

  // ........................................................................ //
  //! @brief remove all whitespaces from a string, in place
  static inline void fullTrim(std::string &s) {
    auto begin = s.begin(),
         end   = s.end();

    for (auto ch = begin; ch != end; ++ch) {
      if (std::isspace(*ch)) {
        s.erase(ch);
        end = s.end();
      }
    }
  }
  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .//
  //! @brief return a copy of the string where all whitespaces are removed
  static inline std::string fullTrim_copy(std::string s) {fullTrim(s); return s;}

  // ------------------------------------------------------------------------ //
  // case conversion

  //! @brief convert the entire string to uppercase characters
  static inline void     to_uppercase(std::string & s) {for (auto & c: s) {c = toupper(c);}          }
  //! @brief return a copy of the string where all characters are converted to uppercase
  static inline std::string uppercase(std::string   s) {for (auto & c: s) {c = toupper(c);} return s;}

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
   *  * \c ? will be interpreted as a wildcard for an arbitrary *single* character
   *  * \c * will be interpreted as a wildcard for an arbitrary *sequence* of characters
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

  //! @brief returns a string representation of a std::complex<double> alias complex_d_t
  template<class T>
  static inline const std::string complex_to_string(const std::complex<T> & z) {
    std::stringstream stream;
    stream << z;
    return stream.str();
  }

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
  // concatenate vectors
  // https://stackoverflow.com/questions/3177241/what-is-the-best-way-to-concatenate-two-vectors

  template<class T>
  static inline std::vector<T> concatenate (const std::vector<T> & A, const std::vector<T> & B) {
    std::vector<T> reVal;

    reVal.insert( reVal.end(), A.begin(), A.end() );
    reVal.insert( reVal.end(), B.begin(), B.end() );

    return reVal;
  }
  // ........................................................................ //
  template<class T>
  static inline void appendTo_vector (std::vector<T> & A, const std::vector<T> & B) {
    A.reserve( A.size() + B.size() );
    A.insert ( A.end(), B.begin(), B.end() );
  }

  // ------------------------------------------------------------------------ //
  // show lists of lists Py-Style

  template<class T>
  static inline std::string vector_to_string(const std::vector<T> & list, bool brackets = true) {
    std::stringstream stream;
    std::string reVal;

    if (list.size() == 0u) {return brackets ? "[]" : "(empty)";}

    if (brackets) {stream << "[";}

    for (auto & e : list) {
      stream << e << ",";
    }

    // remove the last comma
    stream.seekp(-1, stream.cur);
    stream << "]";

    reVal = stream.str();
    if (!brackets) {reVal.pop_back();}

    return reVal;
  }
  // ........................................................................ //
  template<class T>
  static inline std::string vecvec_to_string(const std::vector<std::vector<T>> & listlist) {
    std::stringstream reVal;

    if (listlist.size() == 0u) {return "[.]";}

    reVal << "[";
    for   (auto & e : listlist) {
      reVal << vector_to_string<T>(e) << ",";
    }

    // remove the last comma
    reVal.seekp(-1, reVal.cur);

    reVal << "]";

    return reVal.str();
  }

  // ------------------------------------------------------------------------ //
  // vector distance

  template<class T>
  double vector_distance(const std::vector<T> & A, const std::vector<T> & B) {
    auto l_add            = [] (const T a, const T b) {return a + b;};
    auto l_delta_squared  = [] (const T a, const T b) {T tmp = a - b; return tmp * tmp;};

    if ( A.size() != B.size() ) return T();

    return std::sqrt(
      std::inner_product( A.begin(), A.end(),
                          B.begin(),
                          0.0,
                          l_add,
                          l_delta_squared
      )
    );
  }

  // ------------------------------------------------------------------------ //
  // find nearby

  template<class Iterator, class T>
  Iterator findNearby(Iterator begin, Iterator end,
                      const T & val,
                      double epsilon,
                      double absFunc (double) = std::abs
  ) {
    auto it = begin;
    for (; it != end; ++it) {
      if (absFunc(*it - val) < epsilon) {break;}
    }

    return it;
  }
  // ........................................................................ //
  template<class Iterator, class T>
  int findNearbyIdx(Iterator begin, Iterator end,
                      const T & val,
                      double epsilon,
                      double absFunc (double) = std::abs
  ) {
    auto spot = findNearby(begin, end, val, epsilon, absFunc);
    if (spot == end) {return -1;}
    else             {return std::distance(begin, spot);}
  }

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

  static inline std::fstream openThrow(const std::string & filename, std::ios_base::openmode mode = std::fstream::out) {
    auto reVal = std::fstream(filename, mode);

    if ( !reVal.is_open() ) {
      throw( std::invalid_argument("failed to open '" + filename + "'") );
    }

    return reVal;
  }
  // ........................................................................ //
  static inline std::string  generateFileComments(const std::string & content) {
    std::string reVal;

    reVal += "# ============================================================================ #\n";
    auto lines = splitString(content, '\n');
    for (const auto & line : lines) {reVal += "# " + line + "\n";}
    reVal += "# timestamp: ";
    reVal +=    generateTimestamp() + "\n";
    reVal += "# ============================================================================ #\n\n";

    return reVal;
  }

  //! @}

  // ======================================================================== //
  // misc

  /** @defgroup GlobalsMisc Misc Convenience Functions
   *  @{
   *
   * @brief TODO
   */

  template <typename T>
  static inline T factorial (T n) {
    /* Can be used with GMP types and works just fine. However, GMP classes have their proper methods for this.
     *
     * type unsigned long long int yields results up to n = 20 and then goes into overload (returns zero)
     * type double yields exact results up to n = 22
     *    reaches computation limit after n = 170 and will yield nan:inf beyond (nothrow)
     *    use at own risk above that
     * type long double yields exact results up to n = 25
     */

    if (n <   0) {throw std::invalid_argument("only positive arguments allowed.");}

    T reVal = 1.0;     // using doubles to keep up with the horrible range factorials can reach
    for (T i = 2; i < n + 1; ++i) {reVal *= i;}

    return reVal;
  }

  // ........................................................................ //

  //! @}

}                                                                               // namespace BCG
#endif
