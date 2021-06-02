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
namespace Settings {
  // ======================================================================== //
  // Getters

  template<typename T>
  const std::vector<T> Restriction::getAftParseList () const {
    if (
      aftParseRestrictionType != RestrictionType::AllowedList   &&
      aftParseRestrictionType != RestrictionType::ForbiddenList
    ) {
      throw std::runtime_error(THROWTEXT(
        "    Restriction is not a list but a "s + restrictionTypeNames[static_cast<int>(aftParseRestrictionType)]
      ));
    }
    
    return std::any_cast<std::vector<T>>(aftParseRestriction);
  }
  // ------------------------------------------------------------------------ //
  template<typename T>
  const std::function<bool (const T &)> Restriction::getAftParseFunc () const {
    if (aftParseRestrictionType != RestrictionType::Function) {
      throw std::runtime_error(THROWTEXT(
        "    Restriction is not a user defined verification function but a "s + restrictionTypeNames[static_cast<int>(aftParseRestrictionType)]
      ));
    }
    
    return std::any_cast<std::function<bool (const T &)>>(aftParseRestriction);
  }

  // ======================================================================== //
  // Setters

  template<typename T>
  void Restriction::setAftParseList(const std::vector<T> & list, bool forbiddenList) {
    auto resType = (forbiddenList ? RestrictionType::ForbiddenList : RestrictionType::AllowedList);
    
    aftParseRestrictionType = resType;
    aftParseRestriction     = list;
  }
  // ------------------------------------------------------------------------ //
  template<typename T>
  void Restriction::setAftParseFunction(const std::function<bool (const T &)> uFunc) {
    if ( !uFunc ) {throw std::runtime_error(THROWTEXT("    Uninitialized parsing function"));}
    
    aftParseRestrictionType = RestrictionType::Function;
    aftParseRestriction     = uFunc;
  }
}
// ========================================================================== //
#undef THROWTEXT
