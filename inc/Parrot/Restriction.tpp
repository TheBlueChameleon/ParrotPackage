// ========================================================================= //
// dependencies

// STL
#include <stdexcept>

#include <vector>
#include <any>
#include <string>

// own
#include "Parrot/Definitions.hpp"
#include "Parrot/Restriction.hpp"

// ========================================================================== //
// local macro

#define THROWTEXT(msg) ("RUNTIME EXCEPTION IN "s + (__PRETTY_FUNCTION__) + "\n"s + msg)

// ========================================================================== //
// Getters

template <typename T>
const std::vector<T> Parrot::Restriction::getAftParseValidationList () const {
  if (
    aftParseRestrictionType != RestrictionType::AllowedList   &&
    aftParseRestrictionType != RestrictionType::ForbiddenList
  ) {
    throw Parrot::RestrictionTypeError(THROWTEXT(
      "    Restriction is not a list but a "s + restrictionTypeName(aftParseRestrictionType)
    ));
  }

  return std::any_cast<std::vector<T>>(aftParseRestriction);
}
// -------------------------------------------------------------------------- //
template <typename T>
const std::function<bool (const T &)> Parrot::Restriction::getAftParseValidationFunction () const {
  if (aftParseRestrictionType != RestrictionType::Function) {
    throw Parrot::RestrictionTypeError(THROWTEXT(
      "    Restriction is not a user defined verification function but a "s + restrictionTypeName(aftParseRestrictionType)
    ));
  }

  return std::any_cast<std::function<bool (const T &)>>(aftParseRestriction);
}

// ========================================================================== //
// Setters

template<typename T>
void Parrot::Restriction::setAftParseValidationList(const std::vector<T> & list, bool forbiddenList) {
  switch ( valueTypeIDOf(list) ) {
    case ValueTypeID::StringList  : restrictionValueTypeID = RestrictionValueTypeID::String ; break;
    case ValueTypeID::IntegerList : restrictionValueTypeID = RestrictionValueTypeID::Integer; break;
    case ValueTypeID::RealList    : restrictionValueTypeID = RestrictionValueTypeID::Real   ; break;
    default :
      throw Parrot::ValueTypeError(THROWTEXT("    invalid validation list type"));
      break;
  }

  auto resType = (forbiddenList ? RestrictionType::ForbiddenList : RestrictionType::AllowedList);

  aftParseRestrictionType = resType;
  aftParseRestriction     = list;

  rectify_AftParseValidationList ();
}

// ========================================================================== //

#undef THROWTEXT
