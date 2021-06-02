/* TODO: File descrption
 * 
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
// type alias

typedef std::complex<double> complex_d_t;

// ========================================================================== //
// generic

// -------------------------------------------------------------------------- //
// globals

constexpr double PI = std::atan(1.0)*4;

// ========================================================================== //
// random number generator

extern std::random_device RNG;
extern std::uniform_real_distribution<> rand_phase_distribution;

double get_randPhase();

// ========================================================================== //
// console output convenience

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

struct ConsoleColorsTriple {
  ConsoleColors fore = ConsoleColors::FORE_WHITE;
  ConsoleColors back = ConsoleColors::BACK_BLACK;
  ConsoleColors spc  = ConsoleColors::SPC_NORMAL;
};

static inline void consoleClear()                   {std::cout << "\033[H\033[J";}
static inline void consoleGotoRC(int row, int col)  {std::cout << "\033[" << row << ";" << col << "H";}

void consoleSetcolor (ConsoleColors code);

void utterWarning ( const std::string & text, 
                    const std::string & headline = "Warning",
                    const int indentFirst   = 0,
                    const int indentHanging = 3,
                    const ConsoleColorsTriple & headlineColors = {ConsoleColors::FORE_BRIGHT_RED},
                    const ConsoleColorsTriple & textColors     = {ConsoleColors::FORE_NORMAL}
);

void coutHeadline ( const std::string & text, 
                    const ConsoleColorsTriple & headlineColors = {ConsoleColors::FORE_BRIGHT_RED}
);

// ========================================================================== //
// String utility

// -------------------------------------------------------------------------- //
// Trim String
// Source:https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring

// trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .//
// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .//
// trim from both ends (in place)
static inline void trim(std::string &s) {ltrim(s); rtrim(s);}
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .//
// trim from start (copying)
static inline std::string ltrim_copy(std::string s) {ltrim(s); return s;}
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .//
// trim from end (copying)
static inline std::string rtrim_copy(std::string s) {rtrim(s); return s;}
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .//
// trim from both ends (copying)
static inline std::string trim_copy(std::string s) {trim(s); return s;}

// .......................................................................... //
// adapted: remove all space characters (in place)
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
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .//
// adapted: remove all space characters (copying)
static inline std::string fullTrim_copy(std::string s) {fullTrim(s); return s;}

// -------------------------------------------------------------------------- //
// case conversion

static inline void     to_uppercase(std::string & s) {for (auto & c: s) {c = toupper(c);}          }
static inline std::string uppercase(std::string   s) {for (auto & c: s) {c = toupper(c);} return s;}

// -------------------------------------------------------------------------- //
// split string

std::vector<std::string> splitString(const std::string & s, const char separator = ',');

// .......................................................................... //
// wildcard matching

bool wildcardmatch(const char        * pattern, const char        * searchstring);
bool wildcardmatch(const std::string & pattern, const std::string & searchstring);

// -------------------------------------------------------------------------- //
// complex

template<class T>
static inline const std::string complex_to_string(const std::complex<T> & z) {
  std::stringstream stream;
  stream << z;
  return stream.str();
}

// ========================================================================== //
// vector utility

// -------------------------------------------------------------------------- //
// concatenate vectors
// https://stackoverflow.com/questions/3177241/what-is-the-best-way-to-concatenate-two-vectors

template<class T>
static inline std::vector<T> concatenate (const std::vector<T> & A, const std::vector<T> & B) {
  std::vector<T> reVal;
  
  reVal.insert( reVal.end(), A.begin(), A.end() );
  reVal.insert( reVal.end(), B.begin(), B.end() );
  
  return reVal;
}
// .......................................................................... //
template<class T>
static inline void appendTo_vector (std::vector<T> & A, const std::vector<T> & B) {
  A.reserve( A.size() + B.size() );
  A.insert ( A.end(), B.begin(), B.end() );
}

// -------------------------------------------------------------------------- //
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
// .......................................................................... //
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

// -------------------------------------------------------------------------- //
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

// -------------------------------------------------------------------------- //
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
// .......................................................................... //
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

// ========================================================================== //
// File utilty

std::string generateTimestamp();

static inline std::fstream openThrow(const std::string & filename, std::ios_base::openmode mode = std::fstream::out) {
  auto reVal = std::fstream(filename, mode);
  
  if ( !reVal.is_open() ) {
    throw( std::invalid_argument("failed to open '" + filename + "'") );
  }
  
  return reVal;
}
// .......................................................................... //
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

// ========================================================================== //
// misc

std::vector<double> linspace(const double start, const double end, const int N);

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

// .......................................................................... //

#endif
