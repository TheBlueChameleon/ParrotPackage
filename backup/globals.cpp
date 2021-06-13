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
