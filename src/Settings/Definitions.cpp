// ========================================================================= //
// dependencies

// STL
#include <vector>
#include <string>

// own
#include "Settings/Definitions.hpp"

// ========================================================================== //
// local macro

// ========================================================================== //
// namespace

namespace Settings {

  // ======================================================================== //
  // constants

  const std::vector<std::string> defaultBooleanTextTrue  = {"TRUE", "YES", "ON"};
  const std::vector<std::string> defaultBooleanTextFalse = {"FALSE", "NO", "OFF"};
  // ------------------------------------------------------------------------ //
  const std::vector<std::string> valueTypeNames = {
    "string",
    "integer",
    "real value",
    "boolean",
    "list of strings",
    "list of integers",
    "list of real values",
    "list of booleans"
  };
  // ........................................................................ //
  const std::vector<std::string> restrictionTypeNames = {
    "none",
    "list of allowed values",
    "list of forbidden values",
    "range",
    "user defined verification function"
  };
  // ........................................................................ //
  const std::vector<std::string> restrictionViolationPolicyNames = {
    "throw a RestrictionViolationError",
    "utter a warning via stderr"
  };
}
