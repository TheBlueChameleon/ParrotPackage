/**
 * @defgroup GlobalsGroup Globals Module
 *
 * @{
 * @page Globals
 * @}
 */

/**
 * @page Globals Globals Module
 *
 * @{
 * The *Blue Chameleon Globals* module is a collection of useful general
 *  purpose functions
 *
 * This Module collects a whole bunch of useful functions. They are sortded into
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
 * All symbols and functions of the Blue Chameleon Globals are defined in the
 * BCG namespace.
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

  //! an alias for \c std::complex<double>
  typedef std::complex<double> complex_d_t;

  // ======================================================================== //
  // generic

  /**
   * @brief initializes various some of the constants used by BCG.
   *
   * This will update the value or state of \c isTTY, \c PRGN, \c RNGseed,
   * \c rand_percentage_distribution and \c rand_phase_distribution
   */
  void init();

  // ------------------------------------------------------------------------ //
  // globals

  /**
   * @brief indicates if stdout is a terminal. Needs to be initialized by init()
   *
   * If this is false, then the ConsoleColorsTriple parameters are ignored as
   * well as the consoleSetcolor, consoleClear and consoleGotoRC commands.
   *
   * By default, this value is \c true. init() calls the POSIX function
   * isatty on stdout to determine whether color console features are available.
   */
  extern    bool   isTTY;

  //! @brief the numerical value of @f$\pi@f$
  constexpr double PI = std::atan(1.0) * 4;

  // ======================================================================== //
  // random number generator

  /**
   * @brief a uniform, nondeterministic integer random number generator.
   *
   * Invoke this by calling \c BCG::trueRNG().
   *
   * This uses the urandom device to generate a "true" random output and is used
   * to set the \c BCG::RNGseed value. As the entropy pool is quickly
   * depleted, this should only be used as a source for pseudorandom number
   * generators such as BCG::PRNG.
   */
  extern std::random_device trueRNG;

  /**
   * @brief the seed value used for \c BCG::PRNG. Initialized by init() to the
   * output of trueRNG(). Use this in your log files to reproduce random
   * experiments.
   *
   */
  extern unsigned long long seedRNG;

  /**
   * @brief uniform deterministic integer pseudorandom number generator with
   *  MT19937 implementation. Needs to be initialized by calling init() or
   *  calling reset_PRNG() with a good seed.
   *
   * Invoke this by calling \c BCG::PRNG().
   *
   * This pseudorandom number generator is used by the BCG's proxy functions to
   * the STL PRNG system.
   */
  extern std::mt19937 PRNG;


  /**
   * @brief resets \c BCG::PRNG with a new seed.
   */
  static inline void reset_PRNG(const int seed = 0);


  /**
   * @brief a uniform phase, i.e. a \c double between 0 and @f$2\pi@f$. Needs
   *  to be initialized by calling init() or manually setting it to
   *  <tt>std::uniform_real_distribution<>(0.0, 2 * BCG::PI)</tt>
   */
  extern std::uniform_real_distribution<> rand_phase_distribution;

  /**
   * @brief a uniform phase, i.e. a \c double between 0 and 1. Needs to be
   * initialized by calling init() or manually setting it to
   * <tt>std::uniform_real_distribution<>(0.0, 1.0)</tt>
   */
  extern std::uniform_real_distribution<> rand_percentage_distribution;

  /**
   * @brief return a random \c double between 0 and @f$2\pi@f$ with uniform
   *  distribution. Needs to be initialized by calling init() or manually
   *  setting rand_phase_distribution and calling reset_PRNG() with a good seed.
   */
  static inline double get_randPhase();

  /**
   * @brief return a random \c double between 0 and 1 with uniform distribution.
   *  Needs to be initialized by calling init() or manually
   *  setting rand_phase_distribution and calling reset_PRNG() with a good seed.
   */
  static inline double get_randPercentage();


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

  /**
   * @brief clears the screen.
   *  Does nothing if <tt>BCG::isTTY</tt> is set to false
   */
  static inline void consoleClear();

  /**
   * @brief places the cursor in (row, col).
   *  Does nothing if <tt>BCG::isTTY</tt> is set to false
   *
   * @param row the row where the console cursor should be placed
   * @param col the column where the console cursor should be placed
   */
  static inline void consoleGotoRC(const int row, const int col);

  /**
   * @brief changes the output format for subsequent outputs to stdout.
   *  Does nothing if <tt>BCG::isTTY</tt> is set to false
   */
  void consoleSetcolor (const ConsoleColors code);

  /**
   * @brief puts a warning message on \c stream
   *
   * The created warning will have the format
   * @verbatim
   headline
      text, obeying the input even if it is
      a multiline string with instances of \n
     @endverbatim
   *
   * @param text the text below the headline. May contain instances of \c \\n
   *  to indicate line breaks in the warning message.
   * @param headline the first line of the warning which will be formatted
   *  differently from the rest of the text
   * @param textColors the output format specification for \c text
   * @param headlineColors the output format specification for \c headline
   * @param indentFirst the number of whitespaces to be printed before \e all
   *  lines of the output
   * @param indentHanging the number of whitespaces to be printed before the
   *  text lines, <em>in addition</em> to the \c indentFirst whitespaces that apply
   *  to all lines.
   * @param stream a \c std::ofstream that designates the device onto which the
   *  output warning should be written.
   */
  void writeWarning ( const std::string & text,
                      const std::string & headline = "Warning",
                      const ConsoleColorsTriple & textColors     = {ConsoleColors::FORE_NORMAL},
                      const ConsoleColorsTriple & headlineColors = {ConsoleColors::FORE_BRIGHT_RED},
                      const int indentFirst   = 0,
                      const int indentHanging = 3,
                      std::ostream & stream = std::cerr
  );

  /**
   * @brief puts text in a box and writes it to a stream.
   *
   * @param text the text to be written.<br>
   *  The text should be less than <tt>width - 4</tt> characters wide.
   * @param format the color format to be applied (ignored if
   *  <tt>BCG::isTTY</tt> is set to \c false)
   * @param width the width of the box in characters
   * @param vertical the character used for the vertical boundaries of the box
   * @param horizontal the character used for the horizontal boundaries of the
   *   box
   * @param edge the character to represent the edges of the box
   * @param stream a \c std::ofstream that designates the device onto which the
   *  output warning should be written
   */
  void writeBoxed(const std::string & text,
                  const ConsoleColorsTriple & format = {ConsoleColors::FORE_WHITE, ConsoleColors::BACK_BLACK, ConsoleColors::SPC_BOLD_ON},
                  const int  width      = 80,
                  const char vertical   = '-',
                  const char horizontal = '|',
                  const char edge       = '+',
                  std::ostream & stream = std::cout
  );

  /**
   * @brief Displays subsequent frames of an idle animation on std::cout.
   *  Does nothing if <tt>BCG::isTTY</tt> is set to false
   *
   * This assumes that the cursor is at the beginning of a new line.
   *
   * The frames of the animation are -, \\, | and /, in that order.
   *
   * @param text a short text to be displayed in front of the animation.
   *  must fit in a single line.
   */
  void idleAnimation(const std::string & text = "please be patient");

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

  //! @brief returns a string representation of a std::complex<double> alias complex_d_t
  template<class T>
  static inline const std::string complex_to_string(const std::complex<T> & z);

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

// ========================================================================== //
// template implementations

#include "globals.tpp"

#endif
