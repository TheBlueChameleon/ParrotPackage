// ========================================================================== //
// dependencies

// STL
#include <iostream>

#include <vector>
#include <cstring>
#include <string>
using namespace std::string_literals;

// own
#include "BCG.hpp"
#include "Parrot/Definitions.hpp"
using namespace Parrot;

// ========================================================================== //
// local macro

#define THROWTEXT(msg) ("RUNTIME EXCEPTION IN "s + (__PRETTY_FUNCTION__) + "\n"s + msg)

// ========================================================================== //
// lookups

const std::string Parrot::TypeIDString_String      = BCG::getTypeIDName( PARROT_TYPE(Parrot::ValueTypeID::String     )() );
const std::string Parrot::TypeIDString_Integer     = BCG::getTypeIDName( PARROT_TYPE(Parrot::ValueTypeID::Integer    )() );
const std::string Parrot::TypeIDString_Real        = BCG::getTypeIDName( PARROT_TYPE(Parrot::ValueTypeID::Real       )() );
const std::string Parrot::TypeIDString_Boolean     = BCG::getTypeIDName( PARROT_TYPE(Parrot::ValueTypeID::Boolean    )() );

const std::string Parrot::TypeIDString_StringList  = BCG::getTypeIDName( PARROT_TYPE(Parrot::ValueTypeID::StringList )() );
const std::string Parrot::TypeIDString_IntegerList = BCG::getTypeIDName( PARROT_TYPE(Parrot::ValueTypeID::IntegerList)() );
const std::string Parrot::TypeIDString_RealList    = BCG::getTypeIDName( PARROT_TYPE(Parrot::ValueTypeID::RealList   )() );
const std::string Parrot::TypeIDString_BooleanList = BCG::getTypeIDName( PARROT_TYPE(Parrot::ValueTypeID::BooleanList)() );
// -------------------------------------------------------------------------- //
const std::vector<std::string> Parrot::defaultBooleanTextTrue  = {"TRUE", "YES", "ON"};
const std::vector<std::string> Parrot::defaultBooleanTextFalse = {"FALSE", "NO", "OFF"};
// -------------------------------------------------------------------------- //
const std::string Parrot::valueTypeName(const ValueTypeID & T) {
  switch (T) {
    case ValueTypeID::None        : return "(empty)";
    case ValueTypeID::String      : return "string";
    case ValueTypeID::Integer     : return "integer";
    case ValueTypeID::Real        : return "real value";
    case ValueTypeID::Boolean     : return "boolean";
    case ValueTypeID::StringList  : return "list of strings";
    case ValueTypeID::IntegerList : return "list of integers";
    case ValueTypeID::RealList    : return "list of real values";
    case ValueTypeID::BooleanList : return "list of booleans";
    default                       : return "(invalid state)";
  }
}
// .......................................................................... //
const std::string Parrot::restrictionTypeName(const RestrictionType & T) {
  switch (T) {
    case RestrictionType::None          : return "none";
    case RestrictionType::AllowedList   : return "list of allowed values";
    case RestrictionType::ForbiddenList : return "list of forbidden values";
    case RestrictionType::Range         : return "range";
    case RestrictionType::Function      : return "user defined verification function";
    default                             : return "(invalid state)";
  }
}
// .......................................................................... //
const std::string Parrot::restrictionViolationPolicyName(const RestrictionViolationPolicy & T) {
  switch (T) {
    case RestrictionViolationPolicy::Exception     : return "throw a RestrictionViolationError";
    case RestrictionViolationPolicy::Warning       : return "print a warning to stderr";
    case RestrictionViolationPolicy::WarningRevert : return "print a warning to stderr and revert to the default value";
    default                                        : return "(invalid state)";
  }
}
// -------------------------------------------------------------------------- //
bool Parrot::isTypeCompatibleWithValidityList(const ValueTypeID & valType, const ValueTypeID & listType) {
  if (
    listType != ValueTypeID::StringList  &&
    listType != ValueTypeID::IntegerList &&
    listType != ValueTypeID::RealList
  ) {return false;}

  if (valType == listType)                                                     {return true;}
  if (valType == ValueTypeID::String  && listType == ValueTypeID::StringList ) {return true;}
  if (valType == ValueTypeID::Integer && listType == ValueTypeID::IntegerList) {return true;}
  if (valType == ValueTypeID::Real    && listType == ValueTypeID::RealList   ) {return true;}

  return false;
}
// -------------------------------------------------------------------------- //
const std::string Parrot::restrictionValueTypeIDName (const RestrictionValueTypeID & T) {
  switch (T) {
    case RestrictionValueTypeID::None        : return "no type restriction";
    case RestrictionValueTypeID::String      : return "only compatible with Strings and StringLists";
    case RestrictionValueTypeID::Integer     : return "only compatible with Integers and IntegerLists";
    case RestrictionValueTypeID::Real        : return "only compatible with Reals and RealLists";
    case RestrictionValueTypeID::Numeric     : return "only compatible with Integers, Reals, IntegerLists and RealLists";
    case RestrictionValueTypeID::Boolean     : return "only compatible with Booleans and BooleanLists";
    case RestrictionValueTypeID::StringList  : return "only compatible with StringLists";
    case RestrictionValueTypeID::IntegerList : return "only compatible with IntegerLists";
    case RestrictionValueTypeID::RealList    : return "only compatible with RealLists";
    case RestrictionValueTypeID::BooleanList : return "only compatible with BooleanLists";
    default                                  : return "(invalid state)";
  }
}
// -------------------------------------------------------------------------- //
const std::string Parrot::missingKeywordPolicyName (const MissingKeywordPolicy & T) {
  switch (T) {
    case MissingKeywordPolicy::Ignore    : return "ignore errors";
    case MissingKeywordPolicy::Silent    : return "silently apply defaults";
    case MissingKeywordPolicy::Warning   : return "apply defaults and print warning to stderr";
    case MissingKeywordPolicy::Exception : return "throw a MissingKeywordError";
    default                              : return "(invalid state)";
  }
}

// ========================================================================== //
// type interpreters

const std::string Parrot::getAnyText(const std::any & x, const ValueTypeID & T) {
  switch(T) {
    case ValueTypeID::None        : return "(### no content ###)";
    case ValueTypeID::String      : return                       std::any_cast<PARROT_TYPE(ValueTypeID::String     )>(x) ;
    case ValueTypeID::Integer     : return std::to_string       (std::any_cast<PARROT_TYPE(ValueTypeID::Integer    )>(x));
    case ValueTypeID::Real        : return std::to_string       (std::any_cast<PARROT_TYPE(ValueTypeID::Real       )>(x));
    case ValueTypeID::Boolean     : return                      (std::any_cast<PARROT_TYPE(ValueTypeID::Boolean    )>(x)) ? "true" : "false";
    case ValueTypeID::StringList  : return BCG::vector_to_string(std::any_cast<PARROT_TYPE(ValueTypeID::StringList )>(x));
    case ValueTypeID::IntegerList : return BCG::vector_to_string(std::any_cast<PARROT_TYPE(ValueTypeID::IntegerList)>(x));
    case ValueTypeID::RealList    : return BCG::vector_to_string(std::any_cast<PARROT_TYPE(ValueTypeID::RealList   )>(x));
    case ValueTypeID::BooleanList : {
      std::string reVal;
      for (auto bit : std::any_cast<PARROT_TYPE(ValueTypeID::BooleanList)>(x)) {reVal += (bit ? "1" : "o");}
      return reVal;
    }

    default                     : return "(invalid type): ";
  }
}
// .......................................................................... //
const std::string Parrot::getAnyText(const std::any & x) {
  if      (!x.has_value()                               ) {return getAnyText(x, ValueTypeID::None       );}
  else if ( x.type().name() == TypeIDString_String      ) {return getAnyText(x, ValueTypeID::String     );}
  else if ( x.type().name() == TypeIDString_Integer     ) {return getAnyText(x, ValueTypeID::Integer    );}
  else if ( x.type().name() == TypeIDString_Real        ) {return getAnyText(x, ValueTypeID::Real       );}
  else if ( x.type().name() == TypeIDString_Boolean     ) {return getAnyText(x, ValueTypeID::Boolean    );}
  else if ( x.type().name() == TypeIDString_StringList  ) {return getAnyText(x, ValueTypeID::StringList );}
  else if ( x.type().name() == TypeIDString_IntegerList ) {return getAnyText(x, ValueTypeID::IntegerList);}
  else if ( x.type().name() == TypeIDString_RealList    ) {return getAnyText(x, ValueTypeID::RealList   );}
  else if ( x.type().name() == TypeIDString_BooleanList ) {return getAnyText(x, ValueTypeID::BooleanList);}
  else {throw Parrot::ValueTypeError(THROWTEXT("    type not supported"));}

}
// .......................................................................... //
ValueTypeID Parrot::getAnyValueType(const std::any & x) {
  if      (!x.has_value()                               ) {return ValueTypeID::None       ;}
  else if ( x.type().name() == TypeIDString_String      ) {return ValueTypeID::String     ;}
  else if ( x.type().name() == TypeIDString_Integer     ) {return ValueTypeID::Integer    ;}
  else if ( x.type().name() == TypeIDString_Real        ) {return ValueTypeID::Real       ;}
  else if ( x.type().name() == TypeIDString_Boolean     ) {return ValueTypeID::Boolean    ;}
  else if ( x.type().name() == TypeIDString_StringList  ) {return ValueTypeID::StringList ;}
  else if ( x.type().name() == TypeIDString_IntegerList ) {return ValueTypeID::IntegerList;}
  else if ( x.type().name() == TypeIDString_RealList    ) {return ValueTypeID::RealList   ;}
  else if ( x.type().name() == TypeIDString_BooleanList ) {return ValueTypeID::BooleanList;}
  else {throw Parrot::ValueTypeError(THROWTEXT("    type not supported"));}
}
