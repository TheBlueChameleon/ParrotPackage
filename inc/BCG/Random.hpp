/**
 * @defgroup BCG_Random BCG Random Numbers Module
 *
 * @brief commonly used extensions to the STL random library
 */

#ifndef BCG_RANDOM_HPP
#define BCG_RANDOM_HPP

// ========================================================================= //
// dependencies

#include <random>

// ========================================================================== //

namespace BCG {
  //! @addtogroup BCG_Random
  //! @{

  // ------------------------------------------------------------------------ //
  // globals

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

  // ------------------------------------------------------------------------ //
  // procs

  /**
   * @brief resets \c BCG::PRNG with a new seed.
   */
  static inline void reset_PRNG(const long long seed = 0);

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

  //! @}
}

// ========================================================================== //
// template implementations

#include "BCG/Random.tpp"

#endif
