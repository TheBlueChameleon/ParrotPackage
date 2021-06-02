// ========================================================================= //
// dependencies

// STL
#include <stdexcept>

#include <vector>
#include <any>
#include <string>

// own
#include "Settings/Definitions.hpp"
#include "Settings/Restriction.hpp"

// ========================================================================== //
// local macro

#define THROWTEXT(msg) ("RUNTIME EXCEPTION IN "s + (__PRETTY_FUNCTION__) + "\n"s + msg)

// ========================================================================== //
// Getters

template<typename T>
const std::function<bool (const T &)> Settings::Restriction::getAftParseFunc () const {
  if (aftParseRestrictionType != RestrictionType::Function) {
    throw std::runtime_error(THROWTEXT(
      "    Restriction is not a user defined verification function but a "s + restrictionTypeNames[static_cast<int>(aftParseRestrictionType)]
    ));
  }
  
  return std::any_cast<std::function<bool (const T &)>>(aftParseRestriction);
}

// ========================================================================== //
// Setters

template<typename T>
void Settings::Restriction::setAftParseFunction(const std::function<bool (const T &)> uFunc) {
  aftParseRestrictionType = RestrictionType::Function;
  aftParseRestriction     = uFunc;
}
#undef THROWTEXT
