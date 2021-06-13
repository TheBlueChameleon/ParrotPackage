// ========================================================================= //
// dependencies

// STL
#include <stdexcept>

// ========================================================================== //
// local macro

#define THROWTEXT(msg) (std::string("RUNTIME EXCEPTION IN ") + (__PRETTY_FUNCTION__) + "\n" + msg)

// ========================================================================== //

namespace BCG {
  // ========================================================================== //
  // random number generator

  static inline void reset_PRNG(const long long seed) {
    seedRNG = seed;
    PRNG = std::mt19937(seedRNG);
  }

  static inline double get_randPhase      () {return rand_phase_distribution     (PRNG);}
  static inline double get_randPercentage () {return rand_percentage_distribution(PRNG);}
}

// ========================================================================== //

#undef THROWTEXT
