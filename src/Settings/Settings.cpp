// ========================================================================= //
// dependencies

// STL
#include <stdexcept>

#include <string>
using namespace std::string_literals;


// own
#include "globals.hpp"
#include "Settings/Settings.hpp"

using namespace Settings;

// ========================================================================== //
// local macro

#define THROWTEXT(msg) ("RUNTIME EXCEPTION IN "s + (__PRETTY_FUNCTION__) + "\n"s + msg)

// ========================================================================== //
// CTors
