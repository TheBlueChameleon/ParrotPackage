// ========================================================================= //
// dependencies

// STL
#include <stdexcept>

#include <sstream>

#include <vector>
#include <any>
#include <string>
using namespace std::string_literals;

// own
#include "globals.hpp"
#include "Settings/Definitions.hpp"
#include "Settings/Restriction.hpp"

using namespace Settings;

// ========================================================================== //
// local macro

#define THROWTEXT(msg) ("RUNTIME EXCEPTION IN "s + (__PRETTY_FUNCTION__) + "\n"s + msg)
#define RESTRICTIONTYPENAME(T) (restrictionTypeNames[static_cast<int>(T)])
#define RESTRICTIONVIOLATIONPOLICYNAME(T) (restrictionViolationPolicyNames[static_cast<int>(restrictionViolationPolicy)])

// ========================================================================== //
// CTor, DTor

Restriction::Restriction(
  RestrictionViolationPolicy restrictionViolationPolicy,
  const std::string & restrictionViolationText
) {
  setRestrictionViolationPolicy(restrictionViolationPolicy, restrictionViolationText);
}
// .......................................................................... //
Restriction::Restriction(
  double min, double max,
  RestrictionViolationPolicy  restrictionViolationPolicy,
  const std::string &         restrictionViolationText
) {
  setAftParseRange(min, max);
  setRestrictionViolationPolicy(restrictionViolationPolicy, restrictionViolationText);
}
// ========================================================================== //
// Getters

RestrictionType   Restriction::getPreParseRestrictionType() const {return preParseRestrictionType;}
const std::any &  Restriction::getPreParseRestriction    () const {return preParseRestriction;}
// -------------------------------------------------------------------------- //
RestrictionType   Restriction::getAftParseRestrictionType() const {return aftParseRestrictionType;}
const std::any &  Restriction::getAftParseRestriction    () const {return aftParseRestriction;}
// -------------------------------------------------------------------------- //
const std::pair<double, double> Restriction::getAftParseRange() const {
  if (aftParseRestrictionType != RestrictionType::Range) {
    throw std::runtime_error(THROWTEXT(
      "    Restriction is not a range but a "s + RESTRICTIONTYPENAME(aftParseRestrictionType)
    ));
  }
  
  return std::any_cast<std::pair<double, double>>(aftParseRestriction);
}
// -------------------------------------------------------------------------- //
const std::vector<std::string>  Restriction::getPreParseList () const {
  if (
    preParseRestrictionType != RestrictionType::AllowedList   &&
    preParseRestrictionType != RestrictionType::ForbiddenList
  ) {
    throw std::runtime_error(THROWTEXT(
      "    Restriction is not a list but a "s + RESTRICTIONTYPENAME(preParseRestrictionType)
    ));
  }
  
  return std::any_cast<std::vector<std::string>>(preParseRestriction);
}
// -------------------------------------------------------------------------- //
const std::function<bool (const std::string &)> Restriction::getPreParseFunc () const {
  if (preParseRestrictionType != RestrictionType::Function) {
    throw std::runtime_error(THROWTEXT(
      "    Restriction is not a user defined verification function but a "s + RESTRICTIONTYPENAME(preParseRestrictionType)
    ));
  }
  
  return std::any_cast<std::function<bool (const std::string &)>>(preParseRestriction);
}
// -------------------------------------------------------------------------- //
RestrictionViolationPolicy  Restriction::getRestrictionViolationPolicy() const {return restrictionViolationPolicy;}
const std::string &         Restriction::getRestrictionViolationText  () const {return restrictionViolationText;}

// ========================================================================== //
// Setters

void Restriction::reset() {
  restrictionViolationPolicy = RestrictionViolationPolicy::Exception;
  restrictionViolationText   = "invalid line";
  
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
void Restriction::setAftParseRange(const double min, const double max) {
  aftParseRestrictionType = RestrictionType::Range;
  aftParseRestriction     = std::make_pair(min, max);;
}
// -------------------------------------------------------------------------- //
void Restriction::setPreParseList(const std::vector<std::string> & list, bool forbiddenList) {
  auto resType = (forbiddenList ? RestrictionType::ForbiddenList : RestrictionType::AllowedList);
  
  preParseRestrictionType = resType;
  preParseRestriction     = list;
}
// -------------------------------------------------------------------------- //
void Restriction::setPreParseFunction(const std::function<bool (const std::string &)> & uFunc) {
  if ( !uFunc ) {throw std::runtime_error(THROWTEXT("    Uninitialized parsing function"));}
  
  preParseRestrictionType = RestrictionType::Function;
  preParseRestriction     = uFunc;
}
// -------------------------------------------------------------------------- //
void Restriction::setRestrictionViolationPolicy (RestrictionViolationPolicy P, const std::string & T) {
  restrictionViolationPolicy = P;
  restrictionViolationText   = T;
}
// .......................................................................... //
void Restriction::setViolationWarningText  (const std::string & text) {setRestrictionViolationPolicy(RestrictionViolationPolicy::Warning  , text);}
void Restriction::setViolationExceptionText(const std::string & text) {setRestrictionViolationPolicy(RestrictionViolationPolicy::Exception, text);}

// ========================================================================== //
// Representation

std::string Restriction::to_string() const {
  std::ostringstream reVal;
  
  reVal << "Restriction\n";
  
  reVal << "  Pre-Parsing Restriction: " << RESTRICTIONTYPENAME(preParseRestrictionType) << "\n";
  switch (preParseRestrictionType) {
    case RestrictionType::None :
      break;
      
    case RestrictionType::AllowedList :
    reVal << "    List: " << vector_to_string(std::any_cast<std::vector<std::string>>(preParseRestriction), false) << "\n";
      break;
      
    case RestrictionType::ForbiddenList :
    reVal << "    List: " << vector_to_string(std::any_cast<std::vector<std::string>>(preParseRestriction), false) << "\n";
      break;
      
    case RestrictionType::Range :
      {
        auto range = std::any_cast<std::pair<double, double>>(preParseRestriction);
        reVal << "    Range: " << range.first << " .. " << range.second << "\n";
        reVal << "    ### INVALID STATE ###\n";
      }
      break;
      
    case RestrictionType::Function :
      break;
  }
  
  reVal << "  Post-Parsing Restriction: " << RESTRICTIONTYPENAME(aftParseRestrictionType) << "\n";
  switch (aftParseRestrictionType) {
    case RestrictionType::None :
      break;
      
    case RestrictionType::AllowedList :
    reVal << "    List: " << vector_to_string(std::any_cast<std::vector<std::string>>(aftParseRestriction), false) << "\n";
      break;
      
    case RestrictionType::ForbiddenList :
    reVal << "    List: " << vector_to_string(std::any_cast<std::vector<std::string>>(aftParseRestriction), false) << "\n";
      break;
      
    case RestrictionType::Range :
      {
        auto range = std::any_cast<std::pair<double, double>>(aftParseRestriction);
        reVal << "    Range: " << range.first << " .. " << range.second << "\n";
      }
      break;
      
    case RestrictionType::Function :
      break;
  }
  
  reVal << "  Violation Policy: " << RESTRICTIONVIOLATIONPOLICYNAME(restrictionViolationPolicy) << "\n";
  reVal << "    Message: " << restrictionViolationText << "\n";
  
  return reVal.str();
}
