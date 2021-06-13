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
#include "BCG.hpp"
#include "Parrot/Definitions.hpp"
#include "Parrot/Restriction.hpp"

using namespace Parrot;

// ========================================================================== //
// local macro

#define THROWTEXT(msg) ("RUNTIME EXCEPTION IN "s + (__PRETTY_FUNCTION__) + "\n"s + msg)

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
// .......................................................................... //
Restriction::Restriction(
  const std::vector<std::string> & list,
  bool forbiddenList,
  RestrictionViolationPolicy restrictionViolationPolicy,
  const std::string & restrictionViolationText
) {
  setPreParseValidationList(list, forbiddenList);
  setRestrictionViolationPolicy(restrictionViolationPolicy, restrictionViolationText);
}
// .......................................................................... //
Restriction::Restriction(
  const std::function<bool (const std::string &)> & uFunc,
  RestrictionViolationPolicy restrictionViolationPolicy,
  const std::string & restrictionViolationText
) {
  setPreParseValidationFunction(uFunc);
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
      "    Restriction is not a range but a "s + restrictionTypeName(aftParseRestrictionType)
    ));
  }
  
  return std::any_cast<std::pair<double, double>>(aftParseRestriction);
}
// -------------------------------------------------------------------------- //
const std::vector<std::string>  Restriction::getPreParseValidationList () const {
  if (
    preParseRestrictionType != RestrictionType::AllowedList   &&
    preParseRestrictionType != RestrictionType::ForbiddenList
  ) {
    throw std::runtime_error(THROWTEXT(
      "    Restriction is not a list but a "s + restrictionTypeName(preParseRestrictionType)
    ));
  }
  
  return std::any_cast<std::vector<std::string>>(preParseRestriction);
}
// -------------------------------------------------------------------------- //
const std::function<bool (const std::string &)> Restriction::getPreParseValidationFunction () const {
  if (preParseRestrictionType != RestrictionType::Function) {
    throw std::runtime_error(THROWTEXT(
      "    Restriction is not a user defined verification function but a "s + restrictionTypeName(preParseRestrictionType)
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
void Restriction::setPreParseValidationList(const std::vector<std::string> & list, bool forbiddenList) {
  auto resType = (forbiddenList ? RestrictionType::ForbiddenList : RestrictionType::AllowedList);
  
  preParseRestrictionType = resType;
  preParseRestriction     = list;
}
// -------------------------------------------------------------------------- //
void Restriction::setPreParseValidationFunction(const std::function<bool (const std::string &)> & uFunc) {
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
  
  reVal << "  Pre-Parsing Restriction: " << restrictionTypeName(preParseRestrictionType) << "\n";
  switch (preParseRestrictionType) {
    case RestrictionType::None :
      break;
      
    case RestrictionType::AllowedList :
    reVal << "    List: " << BCG::vector_to_string(std::any_cast<std::vector<std::string>>(preParseRestriction)) << "\n";
      break;
      
    case RestrictionType::ForbiddenList :
    reVal << "    List: " << BCG::vector_to_string(std::any_cast<std::vector<std::string>>(preParseRestriction)) << "\n";
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
  
  reVal << "  Post-Parsing Restriction: " << restrictionTypeName(aftParseRestrictionType) << "\n";
  switch (aftParseRestrictionType) {
    case RestrictionType::None :
      break;
      
    case RestrictionType::AllowedList :
      reVal << "    List: " << "(### given ###)\n";
      // getAnyText(aftParseRestriction, ) + "\n";
      //vector_to_string(std::any_cast<std::vector<std::string>>(aftParseRestriction)) << "\n";
      break;
      
    case RestrictionType::ForbiddenList :
      reVal << "    List: " << "(### given ###)\n";
      //vector_to_string(std::any_cast<std::vector<std::string>>(aftParseRestriction)) << "\n";
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
  
  reVal << "  Violation Policy: " << restrictionViolationPolicyName(restrictionViolationPolicy) << "\n";
  reVal << "    Message: " << restrictionViolationText << "\n";
  
  return reVal.str();
}
