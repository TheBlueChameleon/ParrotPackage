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
// Rectifyers

void Restriction::rectify_AftParseValidationList () {
  auto t_ID = aftParseRestriction.type().name();

  // do nothing if the list type is a parrot native list
  if (
    t_ID == TypeIDString_StringList  ||
    t_ID == TypeIDString_IntegerList ||
    t_ID == TypeIDString_RealList
  ) {return;}

  // otherwise, try to find and convert to an apt list type
  auto type = BCG::demangle( aftParseRestriction.type().name() );

  if        (type == "std::vector<char, std::allocator<char> >"              ) {
    std::vector<PARROT_TYPE(ValueTypeID::Integer)> newVal;

    auto src = std::any_cast< std::vector<char> >(aftParseRestriction);
    newVal.reserve(src.size());
    std::transform(src.begin(), src.end(),
                   std::back_inserter(newVal),
                   [] (auto & v) {return PARROT_TYPE(ValueTypeID::Integer)(v);}
                   );

    aftParseRestriction = newVal;

  } else if (type == "std::vector<short, std::allocator<short> >"            ) {
    std::vector<PARROT_TYPE(ValueTypeID::Integer)> newVal;

    auto src = std::any_cast< std::vector<short> >(aftParseRestriction);
    newVal.reserve(src.size());
    std::transform(src.begin(), src.end(),
                   std::back_inserter(newVal),
                   [] (auto & v) {return PARROT_TYPE(ValueTypeID::Integer)(v);}
                   );

    aftParseRestriction = newVal;

  } else if (type == "std::vector<int, std::allocator<int> >"                ) {
    std::vector<PARROT_TYPE(ValueTypeID::Integer)> newVal;

    auto src = std::any_cast< std::vector<int> >(aftParseRestriction);
    newVal.reserve(src.size());
    std::transform(src.begin(), src.end(),
                   std::back_inserter(newVal),
                   [] (auto & v) {return PARROT_TYPE(ValueTypeID::Integer)(v);}
                   );

    aftParseRestriction = newVal;

  } else if (type == "std::vector<long, std::allocator<long> >"              ) {
    std::vector<PARROT_TYPE(ValueTypeID::Integer)> newVal;

    auto src = std::any_cast< std::vector<long> >(aftParseRestriction);
    newVal.reserve(src.size());
    std::transform(src.begin(), src.end(),
                   std::back_inserter(newVal),
                   [] (auto & v) {return PARROT_TYPE(ValueTypeID::Integer)(v);}
                   );

    aftParseRestriction = newVal;



  } else if (type == "std::vector<float, std::allocator<float> >"            ) {
    std::vector<PARROT_TYPE(ValueTypeID::Real)> newVal;

    auto src = std::any_cast< std::vector<float> >(aftParseRestriction);
    newVal.reserve(src.size());
    std::transform(src.begin(), src.end(),
                   std::back_inserter(newVal),
                   [] (auto & v) {return PARROT_TYPE(ValueTypeID::Real)(v);}
                   );

    aftParseRestriction = newVal;

  } else if (type == "std::vector<long double, std::allocator<long double> >") {
    std::vector<PARROT_TYPE(ValueTypeID::Real)> newVal;

    auto src = std::any_cast< std::vector<long double> >(aftParseRestriction);
    newVal.reserve(src.size());
    std::transform(src.begin(), src.end(),
                   std::back_inserter(newVal),
                   [] (auto & v) {return PARROT_TYPE(ValueTypeID::Real)(v);}
                   );


    aftParseRestriction = newVal;



  } else if (type == "std::vector<char const*, std::allocator<char const*> >") {
    const auto & old = std::any_cast< std::vector<char const *> >(aftParseRestriction);
    aftParseRestriction = std::vector<PARROT_TYPE(ValueTypeID::String)>(old.begin(), old.end());



  } else {throw std::runtime_error(THROWTEXT("    List type "s + type + " not compatible with Parrot."));}
}

// ========================================================================== //
// CTor, DTor

Restriction::Restriction(
  RestrictionViolationPolicy restrictionViolationPolicy,
  const std::string & restrictionViolationText
) {
  setRestrictionViolationPolicy(restrictionViolationPolicy, restrictionViolationText);
}
// .......................................................................... //
Restriction::Restriction(PARROT_TYPE(ValueTypeID::Real) min,
                         PARROT_TYPE(ValueTypeID::Real) max,
                         RestrictionViolationPolicy     restrictionViolationPolicy,
                         const std::string &            restrictionViolationText
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
const std::pair<PARROT_TYPE(ValueTypeID::Real), PARROT_TYPE(ValueTypeID::Real)> Restriction::getAftParseRange() const {
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
void Restriction::setAftParseRange(const PARROT_TYPE(ValueTypeID::Real) min, const PARROT_TYPE(ValueTypeID::Real) max) {
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
  if ( !uFunc ) {throw std::runtime_error(THROWTEXT("    Uninitialized validation function"));}
  
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
      auto obj = std::any_cast<std::function<bool (const std::string &)>>(preParseRestriction);
      auto ptr = obj.target<bool(*) (const std::string &)>();
      reVal << "    user defined function at " << ptr << "\n";
      break;
  }
  
  reVal << "  Post-Parsing Restriction: " << restrictionTypeName(aftParseRestrictionType) << "\n";
  switch (aftParseRestrictionType) {
    case RestrictionType::None :
      break;
      
    case RestrictionType::AllowedList :
      reVal << "    List: " << "(### given ###)" << "\n"; // << BCG::vector_to_string(std::any_cast<std::vector<std::string>>(aftParseRestriction)) << "\n";
      break;
      
    case RestrictionType::ForbiddenList :
      reVal << "    List: " << "(### given ###)" << "\n"; // << BCG::vector_to_string(std::any_cast<std::vector<std::string>>(aftParseRestriction)) << "\n";
      break;
      
    case RestrictionType::Range :
      {
        auto range = std::any_cast<std::pair<double, double>>(aftParseRestriction);
        reVal << "    Range: " << range.first << " .. " << range.second << "\n";
      }
      break;
      
    case RestrictionType::Function :
      // see stackoverflow:
      // https://stackoverflow.com/questions/9065081/how-do-i-get-the-argument-types-of-a-function-pointer-in-a-variadic-template-cla

//       auto obj = std::any_cast<std::function<bool (const std::string &)>>(preParseRestriction);
//       auto ptr = obj.target<bool(*) (const std::string &)>();
//       reVal << "    user defined function at " << ptr << "\n";

      reVal << "    (### given ###)" << "\n";
      break;
  }
  
  reVal << "  Violation Policy: " << restrictionViolationPolicyName(restrictionViolationPolicy) << "\n";
  reVal << "    Message: " << restrictionViolationText << "\n";
  
  return reVal.str();
}
