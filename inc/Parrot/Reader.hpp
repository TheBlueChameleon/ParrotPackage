/* TODO: File descrption
 *
 */

#ifndef PARROT_READER_HPP
#define PARROT_READER_HPP

// ========================================================================== //
// dependencies

// STL
#include <string>
#include <vector>

// own
#include "Parrot/Definitions.hpp"
#include "Parrot/Descriptor.hpp"

// ========================================================================== //

namespace Parrot {

  // ======================================================================== //
  // class

  class Reader {
  private:
    std::vector<Parrot::Descriptor> descriptors;

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
