// ========================================================================= //
// dependencies

// STL
#include <stdexcept>

#include <vector>
#include <any>
#include <string>
using namespace std::string_literals;

// own
#include "Settings/Definitions.hpp"
#include "Settings/Restriction.hpp"

using namespace Settings;

// ========================================================================== //
// local macro

#define THROWTEXT(msg) ("RUNTIME EXCEPTION IN "s + (__PRETTY_FUNCTION__) + "\n"s + msg)

// ========================================================================== //
// CTor, DTor

// ========================================================================== //
// Getters

RestrictionType   Restriction::getPreParseRestrictionType() const {return preParseRestrictionType;}
const std::any &  Restriction::getPreParseRestriction    () const {return preParseRestriction;}
// -------------------------------------------------------------------------- //
RestrictionType   Restriction::getAftParseRestrictionType() const {return aftParseRestrictionType;}
const std::any &  Restriction::getAftParseRestriction    () const {return aftParseRestriction;}
// -------------------------------------------------------------------------- //
const std::vector<std::string>  Restriction::getPreParseList () const {
  if (
    preParseRestrictionType != RestrictionType::AllowedList   &&
    preParseRestrictionType != RestrictionType::ForbiddenList
  ) {
    throw std::runtime_error(THROWTEXT(
      "    Restriction is not a list but a "s + restrictionTypeNames[static_cast<int>(preParseRestrictionType)]
    ));
  }
  
  return std::any_cast<std::vector<std::string>>(preParseRestriction);
}
// .......................................................................... //
const std::vector<std::string>  Restriction::getAftParseList () const {
  if (
    aftParseRestrictionType != RestrictionType::AllowedList   &&
    aftParseRestrictionType != RestrictionType::ForbiddenList
  ) {
    throw std::runtime_error(THROWTEXT(
      "    Restriction is not a list but a "s + restrictionTypeNames[static_cast<int>(aftParseRestrictionType)]
    ));
  }
  
  return std::any_cast<std::vector<std::string>>(aftParseRestriction);
}
// -------------------------------------------------------------------------- //
const std::pair<double, double> Restriction::getPreParseRange() const {
  if (preParseRestrictionType != RestrictionType::Range) {
    throw std::runtime_error(THROWTEXT(
      "    Restriction is not a range but a "s + restrictionTypeNames[static_cast<int>(preParseRestrictionType)]
    ));
  }
  
  return std::any_cast<std::pair<double, double>>(preParseRestriction);
}
// .......................................................................... //
const std::pair<double, double> Restriction::getAftParseRange() const {
  if (aftParseRestrictionType != RestrictionType::Range) {
    throw std::runtime_error(THROWTEXT(
      "    Restriction is not a range but a "s + restrictionTypeNames[static_cast<int>(aftParseRestrictionType)]
    ));
  }
  
  return std::any_cast<std::pair<double, double>>(aftParseRestriction);
}
// -------------------------------------------------------------------------- //
const std::function<bool (const std::string &)> Restriction::getPreParseFunc () const {
  if (preParseRestrictionType != RestrictionType::Function) {
    throw std::runtime_error(THROWTEXT(
      "    Restriction is not a user defined verification function but a "s + restrictionTypeNames[static_cast<int>(preParseRestrictionType)]
    ));
  }
  
  return std::any_cast<std::function<bool (const std::string &)>>(preParseRestriction);
}
// .......................................................................... //
// template<typename T>
// const std::function<bool> Restriction::getAftParseFunc () const {}

// ========================================================================== //
// Setters

void Restriction::reset() {
  resetPreParseRestriction();
  resetAftParseRestriction();
}
// .......................................................................... //
void Restriction::resetPreParseRestriction() {
  preParseRestrictionType = RestrictionType::None;
  preParseRestriction.reset();
}
// .......................................................................... //
void Restriction::resetAftParseRestriction() {
  aftParseRestrictionType = RestrictionType::None;
  aftParseRestriction.reset();
}
// -------------------------------------------------------------------------- //
void Restriction::setPreParseList(const std::vector<std::string> & list, bool forbiddenList) {
  auto resType = (forbiddenList ? RestrictionType::ForbiddenList : RestrictionType::AllowedList);
  
  preParseRestrictionType = resType;
  preParseRestriction     = list;
}
// .......................................................................... //
void Restriction::setAftParseList(const std::vector<std::string> & list, bool forbiddenList) {
  auto resType = (forbiddenList ? RestrictionType::ForbiddenList : RestrictionType::AllowedList);
  
  aftParseRestrictionType = resType;
  aftParseRestriction     = list;
}
