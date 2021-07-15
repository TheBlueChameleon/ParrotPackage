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
  /* You can pass a std::vector<T> as a restriction list, but in the end they
   * have to be regular Parrot types. This function attempts to convert to the
   * supported types.
   *
   * Essentially, this is a long type name lookup.
   */

  auto t_ID = aftParseRestriction.type().name();

  // do nothing if the list type is a parrot native list
  if (
    t_ID == TypeIDString_StringList  ||
    t_ID == TypeIDString_IntegerList ||
    t_ID == TypeIDString_RealList
  ) {return;}

  // otherwise, try to find and convert to an apt list type
  auto type = BCG::demangle( aftParseRestriction.type().name() );

  // ........................................................................ //
  // int types
  if        (type == "std::vector<char, std::allocator<char> >"              ) {
    std::vector<PARROT_TYPE(ValueTypeID::Integer)> newVal;

    auto src = std::any_cast< std::vector<char> >(aftParseRestriction);
    newVal.reserve(src.size());
    std::transform(src.begin(), src.end(),
                   std::back_inserter(newVal),
                   [] (auto & v) {return PARROT_TYPE(ValueTypeID::Integer)(v);}
                   );

    aftParseRestriction    = newVal;

  } else if (type == "std::vector<short, std::allocator<short> >"            ) {
    std::vector<PARROT_TYPE(ValueTypeID::Integer)> newVal;

    auto src = std::any_cast< std::vector<short> >(aftParseRestriction);
    newVal.reserve(src.size());
    std::transform(src.begin(), src.end(),
                   std::back_inserter(newVal),
                   [] (auto & v) {return PARROT_TYPE(ValueTypeID::Integer)(v);}
                   );

    aftParseRestriction    = newVal;

  } else if (type == "std::vector<int, std::allocator<int> >"                ) {
    std::vector<PARROT_TYPE(ValueTypeID::Integer)> newVal;

    auto src = std::any_cast< std::vector<int> >(aftParseRestriction);
    newVal.reserve(src.size());
    std::transform(src.begin(), src.end(),
                   std::back_inserter(newVal),
                   [] (auto & v) {return PARROT_TYPE(ValueTypeID::Integer)(v);}
                   );

    aftParseRestriction    = newVal;

  } else if (type == "std::vector<long, std::allocator<long> >"              ) {
    std::vector<PARROT_TYPE(ValueTypeID::Integer)> newVal;

    auto src = std::any_cast< std::vector<long> >(aftParseRestriction);
    newVal.reserve(src.size());
    std::transform(src.begin(), src.end(),
                   std::back_inserter(newVal),
                   [] (auto & v) {return PARROT_TYPE(ValueTypeID::Integer)(v);}
                   );

    aftParseRestriction    = newVal;


  // ........................................................................ //
  // real types

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


  // ........................................................................ //
  // string types

  } else if (type == "std::vector<char const*, std::allocator<char const*> >") {
    const auto & old = std::any_cast< std::vector<char const *> >(aftParseRestriction);
    aftParseRestriction = std::vector<PARROT_TYPE(ValueTypeID::String)>(old.begin(), old.end());



  } else {throw Parrot::ValueTypeError(THROWTEXT("    List type "s + type + " not compatible with Parrot."));}
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
  const PARROT_TYPE(ValueTypeID::StringList) & list,
  bool forbiddenList,
  RestrictionViolationPolicy restrictionViolationPolicy,
  const std::string & restrictionViolationText
) {
  setPreParseValidationList(list, forbiddenList);
  setRestrictionViolationPolicy(restrictionViolationPolicy, restrictionViolationText);
}
// .......................................................................... //
Restriction::Restriction(
  const std::function<bool (const PARROT_TYPE(ValueTypeID::String) &)> & uFunc,
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
    throw Parrot::RestrictionTypeError(THROWTEXT(
      "    Restriction is not a range but a "s + restrictionTypeName(aftParseRestrictionType)
    ));
  }
  
  return std::any_cast<std::pair<double, double>>(aftParseRestriction);
}
// -------------------------------------------------------------------------- //
const PARROT_TYPE(ValueTypeID::StringList)                                      Restriction::getPreParseValidationList () const {
  if (
    preParseRestrictionType != RestrictionType::AllowedList   &&
    preParseRestrictionType != RestrictionType::ForbiddenList
  ) {
    throw Parrot::RestrictionTypeError(THROWTEXT(
      "    Restriction is not a list but a "s + restrictionTypeName(preParseRestrictionType)
    ));
  }
  
  return std::any_cast<std::vector<std::string>>(preParseRestriction);
}
// -------------------------------------------------------------------------- //
const std::function<bool (const PARROT_TYPE(ValueTypeID::String) &)>    Restriction::getPreParseValidationFunction () const {
  if (preParseRestrictionType != RestrictionType::Function) {
    throw Parrot::RestrictionTypeError(THROWTEXT(
      "    Restriction is not a user defined verification function but a "s + restrictionTypeName(preParseRestrictionType)
    ));
  }

  return std::any_cast<std::function<bool (const std::string &)>>(preParseRestriction);
}
// -------------------------------------------------------------------------- //
RestrictionValueTypeID     Restriction::getRestrictionValueTypeID    () const {return restrictionValueTypeID;}
// -------------------------------------------------------------------------- //
RestrictionViolationPolicy Restriction::getRestrictionViolationPolicy() const {return restrictionViolationPolicy;}
const std::string &        Restriction::getRestrictionViolationText  () const {return restrictionViolationText;}

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
  restrictionValueTypeID  = RestrictionValueTypeID::None;
  aftParseRestrictionType = RestrictionType::None;
  aftParseRestriction.reset();
}
// -------------------------------------------------------------------------- //
void Restriction::setAftParseRange(const PARROT_TYPE(ValueTypeID::Real) min, const PARROT_TYPE(ValueTypeID::Real) max) {
  aftParseRestrictionType = RestrictionType::Range;
  aftParseRestriction     = std::make_pair(min, max);
  restrictionValueTypeID  = RestrictionValueTypeID::Numeric;
}
// -------------------------------------------------------------------------- //
void Restriction::setPreParseValidationList(const PARROT_TYPE(ValueTypeID::StringList) & list, bool forbiddenList) {
  auto resType = (forbiddenList ? RestrictionType::ForbiddenList : RestrictionType::AllowedList);
  
  preParseRestrictionType = resType;
  preParseRestriction     = list;
}
// -------------------------------------------------------------------------- //
void Restriction::setPreParseValidationFunction(const std::function<bool (const PARROT_TYPE(ValueTypeID::String) &)> & uFunc) {
  /* This only checks whether uFunc has ever been set or was obtained from an
   * empty CTor.
   */
  if ( !uFunc ) {throw Parrot::InvalidFunctionError(THROWTEXT("    Uninitialized validation function"));}

  /* Apparently, it is possible to pass wrong-typed functions without any issue.
   * Only then, retrieving the function pointer will yield zero and the call
   * itself will fail.
   */
  auto ptr = (void *) uFunc.target<bool(*) (const PARROT_TYPE(ValueTypeID::String) &)>();
  if ( !ptr   ) {throw Parrot::InvalidFunctionError(THROWTEXT("    Uninitialized validation function"));}

  preParseRestrictionType = RestrictionType::Function;
  preParseRestriction     = uFunc;
}
// -------------------------------------------------------------------------- //
/* These functions below are given type-explicit for two reasons:
 * * functions need to be re-cast from any. Getting that right from the typeID
 *    is ridiculously hard to get right, with all the const, ref, ... degrees of
 *    freedom. Like this, I just fix the form
 * * They actually differ by what restrictionValueTypeID they set. Generalizing
 *    this is just not worth it.
 */
// .......................................................................... //
void Restriction::setAftParseValidationFunction(const std::function<bool (const PARROT_TYPE(ValueTypeID::String     ) &)> & uFunc) {
  if ( !uFunc ) {throw Parrot::InvalidFunctionError(THROWTEXT("    Uninitialized parsing function"));}

  auto ptr = (void *) uFunc.target<bool(*) (const PARROT_TYPE(ValueTypeID::String) &)>();
  if ( !ptr   ) {throw Parrot::InvalidFunctionError(THROWTEXT("    Uninitialized validation function"));}

  aftParseRestrictionType = RestrictionType::Function;
  aftParseRestriction     = uFunc;
  restrictionValueTypeID  = RestrictionValueTypeID::String;
}
// .......................................................................... //
void Restriction::setAftParseValidationFunction(const std::function<bool (const PARROT_TYPE(ValueTypeID::Integer    ) &)> & uFunc) {
  if ( !uFunc ) {throw Parrot::InvalidFunctionError(THROWTEXT("    Uninitialized parsing function"));}

  auto ptr = (void *) uFunc.target<bool(*) (const PARROT_TYPE(ValueTypeID::Integer) &)>();
  if ( !ptr   ) {throw Parrot::InvalidFunctionError(THROWTEXT("    Uninitialized validation function"));}

  aftParseRestrictionType = RestrictionType::Function;
  aftParseRestriction     = uFunc;
  restrictionValueTypeID  = RestrictionValueTypeID::Integer;
}
// .......................................................................... //
void Restriction::setAftParseValidationFunction(const std::function<bool (const PARROT_TYPE(ValueTypeID::Real       ) &)> & uFunc) {
  if ( !uFunc ) {throw Parrot::InvalidFunctionError(THROWTEXT("    Uninitialized parsing function"));}

  auto ptr = (void *) uFunc.target<bool(*) (const PARROT_TYPE(ValueTypeID::Real) &)>();
  if ( !ptr   ) {throw Parrot::InvalidFunctionError(THROWTEXT("    Uninitialized validation function"));}

  aftParseRestrictionType = RestrictionType::Function;
  aftParseRestriction     = uFunc;
  restrictionValueTypeID  = RestrictionValueTypeID::Real;
}
// .......................................................................... //
void Restriction::setAftParseValidationFunction(const std::function<bool (const PARROT_TYPE(ValueTypeID::Boolean    ) &)> & uFunc) {
  if ( !uFunc ) {throw Parrot::InvalidFunctionError(THROWTEXT("    Uninitialized parsing function"));}

  auto ptr = (void *) uFunc.target<bool(*) (const PARROT_TYPE(ValueTypeID::Boolean) &)>();
  if ( !ptr   ) {throw Parrot::InvalidFunctionError(THROWTEXT("    Uninitialized validation function"));}

  aftParseRestrictionType = RestrictionType::Function;
  aftParseRestriction     = uFunc;
  restrictionValueTypeID  = RestrictionValueTypeID::Boolean;
}
// .......................................................................... //
void Restriction::setAftParseValidationFunction(const std::function<bool (const PARROT_TYPE(ValueTypeID::StringList ) &)> & uFunc) {
  if ( !uFunc ) {throw Parrot::InvalidFunctionError(THROWTEXT("    Uninitialized parsing function"));}

  auto ptr = (void *) uFunc.target<bool(*) (const PARROT_TYPE(ValueTypeID::StringList) &)>();
  if ( !ptr   ) {throw Parrot::InvalidFunctionError(THROWTEXT("    Uninitialized validation function"));}

  aftParseRestrictionType = RestrictionType::Function;
  aftParseRestriction     = uFunc;
  restrictionValueTypeID  = RestrictionValueTypeID::StringList;
}
// .......................................................................... //
void Restriction::setAftParseValidationFunction(const std::function<bool (const PARROT_TYPE(ValueTypeID::IntegerList) &)> & uFunc) {
  if ( !uFunc ) {throw Parrot::InvalidFunctionError(THROWTEXT("    Uninitialized parsing function"));}

  auto ptr = (void *) uFunc.target<bool(*) (const PARROT_TYPE(ValueTypeID::IntegerList) &)>();
  if ( !ptr   ) {throw Parrot::InvalidFunctionError(THROWTEXT("    Uninitialized validation function"));}

  aftParseRestrictionType = RestrictionType::Function;
  aftParseRestriction     = uFunc;
  restrictionValueTypeID  = RestrictionValueTypeID::IntegerList;
}
// .......................................................................... //
void Restriction::setAftParseValidationFunction(const std::function<bool (const PARROT_TYPE(ValueTypeID::RealList   ) &)> & uFunc) {
  if ( !uFunc ) {throw Parrot::InvalidFunctionError(THROWTEXT("    Uninitialized parsing function"));}

  auto ptr = (void *) uFunc.target<bool(*) (const PARROT_TYPE(ValueTypeID::RealList) &)>();
  if ( !ptr   ) {throw Parrot::InvalidFunctionError(THROWTEXT("    Uninitialized validation function"));}

  aftParseRestrictionType = RestrictionType::Function;
  aftParseRestriction     = uFunc;
  restrictionValueTypeID  = RestrictionValueTypeID::RealList;
}
// .......................................................................... //
void Restriction::setAftParseValidationFunction(const std::function<bool (const PARROT_TYPE(ValueTypeID::BooleanList) &)> & uFunc) {
  if ( !uFunc ) {throw Parrot::InvalidFunctionError(THROWTEXT("    Uninitialized parsing function"));}

  auto ptr = (void *) uFunc.target<bool(*) (const PARROT_TYPE(ValueTypeID::BooleanList) &)>();
  if ( !ptr   ) {throw Parrot::InvalidFunctionError(THROWTEXT("    Uninitialized validation function"));}

  aftParseRestrictionType = RestrictionType::Function;
  aftParseRestriction     = uFunc;
  restrictionValueTypeID  = RestrictionValueTypeID::BooleanList;
}
// -------------------------------------------------------------------------- //
void Restriction::setRestrictionViolationPolicy (RestrictionViolationPolicy P, const std::string & T) {
  restrictionViolationPolicy = P;
  restrictionViolationText   = T;
}
// .......................................................................... //
void Restriction::setViolationWarningText       (const std::string & text) {setRestrictionViolationPolicy(RestrictionViolationPolicy::Warning      , text);}
void Restriction::setViolationWarningRevertText (const std::string & text) {setRestrictionViolationPolicy(RestrictionViolationPolicy::WarningRevert, text);}
void Restriction::setViolationExceptionText     (const std::string & text) {setRestrictionViolationPolicy(RestrictionViolationPolicy::Exception    , text);}

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
      reVal << "    List: " << BCG::vector_to_string(std::any_cast<PARROT_TYPE(ValueTypeID::StringList)>(preParseRestriction)) << "\n";
      break;
      
    case RestrictionType::ForbiddenList :
      reVal << "    List: " << BCG::vector_to_string(std::any_cast<PARROT_TYPE(ValueTypeID::StringList)>(preParseRestriction)) << "\n";
      break;
      
    case RestrictionType::Range :
      {
        auto range = std::any_cast<std::pair<PARROT_TYPE(ValueTypeID::Real), PARROT_TYPE(ValueTypeID::Real)>>(preParseRestriction);
        reVal << "    Range: " << range.first << " .. " << range.second << "\n";
        reVal << "    ### INVALID STATE ###\n";
      }
      break;
      
    case RestrictionType::Function :
      auto obj = std::any_cast<std::function<bool    (const PARROT_TYPE(ValueTypeID::String) &)>>(preParseRestriction);
      auto ptr = obj.target                 <bool(*) (const PARROT_TYPE(ValueTypeID::String) &)>();
      reVal << "    user defined function at " << (void*) *ptr << "\n";
      break;
  }
  
  reVal << "  Post-Parsing Restriction: " << restrictionTypeName(aftParseRestrictionType) << "\n";
  switch (aftParseRestrictionType) {
    case RestrictionType::None :
      break;
      
    case RestrictionType::AllowedList :
    {
      reVal << "    List: ";
      auto type = aftParseRestriction.type().name();
      if      (type == TypeIDString_StringList ) {reVal << BCG::vector_to_string(std::any_cast<PARROT_TYPE(ValueTypeID::StringList )>(aftParseRestriction));}
      else if (type == TypeIDString_IntegerList) {reVal << BCG::vector_to_string(std::any_cast<PARROT_TYPE(ValueTypeID::IntegerList)>(aftParseRestriction));}
      else if (type == TypeIDString_RealList   ) {reVal << BCG::vector_to_string(std::any_cast<PARROT_TYPE(ValueTypeID::RealList   )>(aftParseRestriction));}
      else                                       {reVal << "### INVALID STATE ###";}
      reVal << "\n";
      break;
    }
      
    case RestrictionType::ForbiddenList :
    {
      reVal << "    List: ";
      auto type = aftParseRestriction.type().name();
      if      (type == TypeIDString_StringList ) {reVal << BCG::vector_to_string(std::any_cast<PARROT_TYPE(ValueTypeID::StringList )>(aftParseRestriction));}
      else if (type == TypeIDString_IntegerList) {reVal << BCG::vector_to_string(std::any_cast<PARROT_TYPE(ValueTypeID::IntegerList)>(aftParseRestriction));}
      else if (type == TypeIDString_RealList   ) {reVal << BCG::vector_to_string(std::any_cast<PARROT_TYPE(ValueTypeID::RealList   )>(aftParseRestriction));}
      else                                       {reVal << "### INVALID STATE ###";}
      reVal << "\n";
      break;
    }
      
    case RestrictionType::Range :
      {
        auto range = std::any_cast<std::pair<double, double>>(aftParseRestriction);
        reVal << "    Range: " << range.first << " .. " << range.second << "\n";
      }
      break;
      
    case RestrictionType::Function :
      switch (restrictionValueTypeID) {
        case RestrictionValueTypeID::String      :
        {
          auto obj = std::any_cast<std::function<bool    (const PARROT_TYPE(ValueTypeID::String) &)>>(aftParseRestriction);
          auto ptr = obj.target                 <bool(*) (const PARROT_TYPE(ValueTypeID::String) &)>();
          reVal << "    user defined function at " << (void*) *ptr;
          break;
        }
        case RestrictionValueTypeID::Integer     :
        {
          auto obj = std::any_cast<std::function<bool    (const PARROT_TYPE(ValueTypeID::Integer) &)>>(aftParseRestriction);
          auto ptr = obj.target                 <bool(*) (const PARROT_TYPE(ValueTypeID::Integer) &)>();
          reVal << "    user defined function at " << (void*) *ptr;
          break;
        }
        case RestrictionValueTypeID::Real        :
        {
          auto obj = std::any_cast<std::function<bool    (const PARROT_TYPE(ValueTypeID::Real) &)>>(aftParseRestriction);
          auto ptr = obj.target                 <bool(*) (const PARROT_TYPE(ValueTypeID::Real) &)>();
          reVal << "    user defined function at " << (void*) *ptr;
          break;
        }
        case RestrictionValueTypeID::Boolean     :
        {
          auto obj = std::any_cast<std::function<bool    (const PARROT_TYPE(ValueTypeID::Boolean) &)>>(aftParseRestriction);
          auto ptr = obj.target                 <bool(*) (const PARROT_TYPE(ValueTypeID::Boolean) &)>();
          reVal << "    user defined function at " << (void*) *ptr;
          break;
        }
        case RestrictionValueTypeID::StringList  :
        {
          auto obj = std::any_cast<std::function<bool    (const PARROT_TYPE(ValueTypeID::StringList) &)>>(aftParseRestriction);
          auto ptr = obj.target                 <bool(*) (const PARROT_TYPE(ValueTypeID::StringList) &)>();
          reVal << "    user defined function at " << (void*) *ptr;
          break;
        }
        case RestrictionValueTypeID::IntegerList :
        {
          auto obj = std::any_cast<std::function<bool    (const PARROT_TYPE(ValueTypeID::IntegerList) &)>>(aftParseRestriction);
          auto ptr = obj.target                 <bool(*) (const PARROT_TYPE(ValueTypeID::IntegerList) &)>();
          reVal << "    user defined function at " << (void*) *ptr;
          break;
        }
        case RestrictionValueTypeID::RealList    :
        {
          auto obj = std::any_cast<std::function<bool    (const PARROT_TYPE(ValueTypeID::RealList) &)>>(aftParseRestriction);
          auto ptr = obj.target                 <bool(*) (const PARROT_TYPE(ValueTypeID::RealList) &)>();
          reVal << "    user defined function at " << (void*) *ptr;
          break;
        }
        case RestrictionValueTypeID::BooleanList :
        {
          auto obj = std::any_cast<std::function<bool    (const PARROT_TYPE(ValueTypeID::BooleanList) &)>>(aftParseRestriction);
          auto ptr = obj.target                 <bool(*) (const PARROT_TYPE(ValueTypeID::BooleanList) &)>();
          reVal << "    user defined function at " << (void*) *ptr;
          break;
        }
        default          :
          reVal << "### INVALID STATE ###";
          break;
      }

      reVal << "\n";
      break;
  }
  
  reVal << "  Type Restrictions: " << restrictionValueTypeIDName(restrictionValueTypeID) << "\n";
  reVal << "  Violation Policy: " << restrictionViolationPolicyName(restrictionViolationPolicy) << "\n";
  reVal << "    Message: " << restrictionViolationText << "\n";
  
  return reVal.str();
}
