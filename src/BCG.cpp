// ========================================================================= //
// dependencies

// STL
#include <stdexcept>

// unix terminal
#include <unistd.h>

// own
#include "BCG.hpp"

// ========================================================================== //
// local macro

#define THROWTEXT(msg) (std::string("RUNTIME EXCEPTION IN ") + (__PRETTY_FUNCTION__) + "\n" + msg)

// ========================================================================== //
// proc

void BCG::init() {
  isTTY = isatty(fileno(stdout));
  reset_PRNG( trueRNG() );

  rand_phase_distribution       = std::uniform_real_distribution<>(0.0, 6.28318530718);   // symbol BCG::PI depends on BCG_MATHS being set
  rand_percentage_distribution  = std::uniform_real_distribution<>(0.0, 1.0          );
}


