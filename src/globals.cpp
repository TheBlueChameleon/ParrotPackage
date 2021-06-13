// ========================================================================== //
// dependencies

// STL
#include <stdexcept>

#include <vector>
#include <string>
using namespace std::string_literals;

#include <cmath>

// // GMP
// #include <gmp.h>
// #include <gmpxx.h>

// own
#include "globals.hpp"

// ========================================================================== //
// local macro

#define THROWTEXT(msg) ("RUNTIME EXCEPTION IN "s + (__PRETTY_FUNCTION__) + "\n"s + msg)

// ========================================================================== //
// namespace

namespace BCG {                                                                 // blue chameleon globals
  // ========================================================================== //
  // generic

  // ------------------------------------------------------------------------ //
  // globals



  // ------------------------------------------------------------------------ //
  // proc



  // ========================================================================== //
  // random number generator




  // ------------------------------------------------------------------------ //
  // proc

  // ========================================================================== //
  // console output convenience

  // ========================================================================== //
  // String utility

  std::vector<std::string> splitString(const std::string & s, const char separator) {
    std::vector<std::string> reVal;
    if ( s.empty() ) {return reVal;}

    auto posSeparator = s.find(separator);
    auto last = 0u;

    while (posSeparator != std::string::npos) {
      reVal.push_back( s.substr(last, posSeparator - last) );

      last = posSeparator + 1;
      posSeparator = s.find(separator, posSeparator + 1);
    }

    if (last != s.size()) {
      reVal.push_back( s.substr(last, s.size() - last) );
    }

    return reVal;
  }
  // .......................................................................... //
  bool wildcardmatch(const char *first, const char * second) {
    // https://www.geeksforgeeks.org/wildcard-character-matching/

    if (*first == '\0' && *second == '\0')                        { return true ; }
    if (*first == '*'  && *(first+1) != '\0' && *second == '\0')  { return false; }
    if (*first == '?'  || *first == *second)                      { return wildcardmatch(first+1, second+1); }
    if (*first == '*')                                            { return wildcardmatch(first+1, second) || wildcardmatch(first, second+1); }
    return false;
  }
  bool wildcardmatch(const std::string & pattern, const std::string & searchstring) {return wildcardmatch(pattern.c_str(), searchstring.c_str());}

  // ========================================================================== //
  // File utility

  // .......................................................................... //
  std::string  generateTimestamp() {
    std::string reVal;

    // adapted from https://stackoverflow.com/questions/16357999/current-date-and-time-as-string
    time_t      rawtime  = time     ( nullptr);
    struct tm * timeinfo = localtime(&rawtime);

    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d, %H:%M:%S", timeinfo);

    reVal = buffer;

    return reVal;
  }

  // ========================================================================== //
  // misc

  std::vector<double> linspace(const double start, const double end, const int N) {
    std::string errText;

    if (N < 0) {
      errText  = "Invalid argument in linspace:\n";
      errText += "\tparameter 'N' needs to be greater than zero!";
      throw std::invalid_argument(errText);
    }

    if ( std::isnan(start) || std::isnan(end) ) {
      errText  = "Invalid argument in linspace:\n";
      errText += "\tEither 'start' or 'end' is NAN!";
      throw std::invalid_argument(errText);
    }

    std::vector<double> reVal(N);

    if (N == 1) {reVal[0] = start; return reVal;}

    auto delta = (end - start) / (N-1);
    auto alpha = start;

    for (auto i=0; i<N; ++i) {
      reVal[i] = alpha;
      alpha += delta;
    }

    return reVal;
  }

}                                                                               // namespace BCG
