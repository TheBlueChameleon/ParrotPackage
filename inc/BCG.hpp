/**
 * @defgroup BCG_Main Blue Chameleon Globals Module (BCG)
 *
 * @brief General Purpose Routine Collection
 *
 * @{
 * @page BCG_About
 * @}
 */

/**
 * @page BCG_About About BCG
 *
 * @{
 * The <em>Blue Chameleon Globals</em> module (BCG) is a collection of useful
 * general purpose functions
 *
 * This Module collects a whole bunch of useful functions. They are sortded into
 * groups pertaining to the tasks they help achieve. See the following pages for
 * details on their effects:
 *
 * - @subpage BCG_Console
 * - @subpage BCG_Maths
 * - @subpage BCG_Random
 *
 * - @subpage GlobalsConsole
 * - @subpage GlobalsString
 * - @subpage GlobalsVector
 * - @subpage GlobalsFiles
 * - @subpage GlobalsMisc
 * - @subpage GlobalSymbols
 *
 * All symbols and functions of the <em>Blue Chameleon Globals</em> module are
 * defined in the BCG namespace.
 *
 * By default, all of them are loaded. If you want to reduce compile time and/or
 * binary size, you may opt to include only some of the submodules. Do so by
 * #defining either of these flags before #including BCG:
 *
 * * \c BCG_RANDOM
 * * \c BCG_MATHS
 * * \c BCG_VECTOR
 * * \c BCG_STRING
 * * \c BCG_CONSOLE (will load \c BCG_STRING as well)
 * * \c BCG_FILES
 *
 * @todo finish!
 * @}
 */

#ifndef BCG_HPP
#define BCG_HPP

// ========================================================================= //
// component selector

# if  !defined(BCG_RANDOM)  && \
      !defined(BCG_MATHS)   && \
      !defined(BCG_VECTOR)  && \
      !defined(BCG_STRING)  && \
      !defined(BCG_CONSOLE) && \
      !defined(BCG_FILES)
#   define BCG_ALL
# endif

# if defined(BCG_ALL)
#   define BCG_RANDOM
#   define BCG_MATHS
#   define BCG_VECTOR
#   define BCG_STRING
#   define BCG_CONSOLE
#   define BCG_FILES
# endif

# if defined(BCG_CONSOLE) && !defined(BCG_STRING)
#   define BCG_STRING
# endif

// ========================================================================= //
// component loader

# if defined(BCG_RANDOM)
#   include "BCG/Random.hpp"
# endif

# if defined(BCG_MATHS)
#   include "BCG/Maths.hpp"
# endif

# if defined(BCG_CONSOLE)
#   include "BCG/Console.hpp"
# endif

// ========================================================================= //
// core components

namespace BCG {
  //! @addtogroup BCG_Main
  //! @{

  /**
   * @brief initializes various some of the constants used by BCG.
   *
   * This will update the value or state of \c isTTY, \c PRGN, \c RNGseed,
   * \c rand_percentage_distribution and \c rand_phase_distribution
   */
  void init();

  //! @}
}
#endif

