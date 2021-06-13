// ========================================================================= //
// dependencies

// STL
#include <stdexcept>

#include <iostream>

// own
#include "BCG.hpp"

// ========================================================================== //
// local macro

#define THROWTEXT(msg) (std::string("RUNTIME EXCEPTION IN ") + (__PRETTY_FUNCTION__) + "\n" + msg)

// ========================================================================== //
// procs

using namespace BCG;

std::vector<double> BCG::linspace(const double start, const double end, const int N) {
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
