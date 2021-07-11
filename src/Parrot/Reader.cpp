// ========================================================================= //
// dependencies

// STL
#include <stdexcept>

#include <string>
using namespace std::string_literals;


// own
#include "BCG.hpp"
#include "Parrot/Reader.hpp"
#include "Parrot/FileContent.hpp"

using namespace Parrot;

// ========================================================================== //
// local macro

#define THROWTEXT(msg) ("RUNTIME EXCEPTION IN "s + (__PRETTY_FUNCTION__) + "\n"s + msg)

// ========================================================================== //
// CTors

// Parrot::Reader::Reader() {
//
//   FileContent f;
//
//   f.x = 1;
// }
