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
  if (N < 0) {
    throw std::invalid_argument(THROWTEXT("    parameter 'N' needs to be greater than zero!"));
  }

  if ( std::isnan(start) || std::isnan(end) ) {
    throw std::invalid_argument(THROWTEXT("    Either 'start' or 'end' is NAN!"));
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
// -------------------------------------------------------------------------- //
std::vector<double> BCG::arange(const double start, const double end, const double inc) {
  if (inc == 0) {
    throw std::invalid_argument(THROWTEXT("    parameter 'inc' must be nonzero!"));
  }

  // compare sign of (end - start) with that of inc to decide whether
  // arangement is possible
  if ( ((end - start) > 0) != (inc > 0) ) {
    THROWTEXT("    either start > end while inc > 0 or end > start while inc < 0!");
  }

  std::vector<double> reVal;
  double x = start;

  while (
    ((inc > 0) && (x < end)) ||
    ((inc < 0) && (x > end))
  ) {
    reVal.push_back(x);
    x += inc;
  }

  return reVal;
}
