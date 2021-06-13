/**
 * @defgroup BCG_Files BCG Files Module
 *
 * @brief commonly used functions when dealing with files
 */

#ifndef BCG_FILES_HPP
#define BCG_FILES_HPP

// ========================================================================= //
// dependencies

#include <stdexcept>

#include <string>
#include <fstream>

// ========================================================================== //

namespace BCG {
  //! @addtogroup BCG_Files
  //! @{

  std::string generateTimestamp();

  static inline std::fstream openThrow(const std::string & filename, std::ios_base::openmode mode = std::fstream::out);

  // ........................................................................ //

  static inline std::string  generateFileComments(const std::string & content);

  //! @}
}

// ========================================================================== //
// template implementations

#include "BCG/Files.tpp"

#endif
