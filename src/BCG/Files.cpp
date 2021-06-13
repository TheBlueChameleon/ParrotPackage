// ========================================================================= //
// dependencies

// STL
#include <stdexcept>
#include <ctime>

// own
#include "BCG.hpp"

// ========================================================================== //
// local macro

#define THROWTEXT(msg) (std::string("RUNTIME EXCEPTION IN ") + (__PRETTY_FUNCTION__) + "\n" + msg)

// ========================================================================== //
// procs

using namespace BCG;

std::string BCG::generateTimestamp() {
  std::string reVal;

  // adapted from https://stackoverflow.com/questions/16357999/current-date-and-time-as-string
  std::time_t rawtime  = time     ( nullptr);
  std::tm *   timeinfo = localtime(&rawtime);

  char buffer[80];
  std::strftime(buffer, sizeof(buffer), "%Y-%m-%d, %H:%M:%S", timeinfo);

  reVal = buffer;

  return reVal;
}
