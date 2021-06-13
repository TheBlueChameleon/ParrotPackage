// ========================================================================= //
// dependencies

// STL
#include <stdexcept>
#include <random>

// ========================================================================== //
// local macro

#define THROWTEXT(msg) (std::string("RUNTIME EXCEPTION IN ") + (__PRETTY_FUNCTION__) + "\n" + msg)

// ========================================================================== //
// globals

namespace BCG {
  unsigned long long seedRNG = 0;

  std::random_device trueRNG;
  std::mt19937          PRNG;

  std::uniform_real_distribution<> rand_phase_distribution;
  std::uniform_real_distribution<> rand_percentage_distribution;
}
