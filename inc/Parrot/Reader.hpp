/* TODO: File descrption
 *
 */

#ifndef PARROT_SETTINGS_HPP
#define PARROT_SETTINGS_HPP

// ========================================================================== //
// dependencies

// STL
#include <string>
#include <vector>

// own
#include "Parrot/Definitions.hpp"
#include "Parrot/Descrptor.hpp"

// ========================================================================== //

namespace Parrot {

  // ======================================================================== //
  // class

  class Reader {
  private:
    std::vector<Parrot::Descrptor> descriptors;

    char                            commentMarker = '#';

    bool verbose = true;

  public:
    // ---------------------------------------------------------------------- //
    // CTors

    Reader() = default;

    // ---------------------------------------------------------------------- //
    // Getters

    // ---------------------------------------------------------------------- //
    // Setters

    // ---------------------------------------------------------------------- //
    // Representation

    std::string to_string() const;
  };
}

// ========================================================================== //

#endif
