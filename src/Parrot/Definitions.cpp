// ========================================================================= //
// dependencies

// STL
#include <iostream>

#include <vector>
#include <cstring>
#include <string>

// own
#include "BCG.hpp"
#include "Parrot/Definitions.hpp"

// ========================================================================== //
// local macro

// ========================================================================== //
// namespace

namespace Parrot {

  // ======================================================================== //
  // lookups

  const std::vector<std::string> defaultBooleanTextTrue  = {"TRUE", "YES", "ON"};
  const std::vector<std::string> defaultBooleanTextFalse = {"FALSE", "NO", "OFF"};
  // ------------------------------------------------------------------------ //
  const std::string valueTypeName(const ValueTypeID & T) {
    switch (T) {
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
  // ........................................................................ //
  const std::string restrictionTypeName(const RestrictionType & T) {
    switch (T) {
      case RestrictionType::None          : return "none";
      case RestrictionType::AllowedList   : return "list of allowed values";
      case RestrictionType::ForbiddenList : return "list of forbidden values";
      case RestrictionType::Range         : return "range";
      case RestrictionType::Function      : return "user defined verification function";
      default                             : return "(invalid state)";
    }
  }
  // ........................................................................ //
  const std::string restrictionViolationPolicyName(const RestrictionViolationPolicy & T) {
    switch (T) {
      case RestrictionViolationPolicy::Exception : return "throw a RestrictionViolationError";
      case RestrictionViolationPolicy::Warning   : return "utter a warning via stderr";
      default                                    : return "(invalid state)";
    }
  }
  // ------------------------------------------------------------------------ //
  bool isTypeCompatibleWithValidityList(const ValueTypeID & valType, const ValueTypeID & listType) {
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

  // ======================================================================== //
  // type interpreters

  const std::string getTypeIDOf(const std::any & x) {return x.type().name();}
  // ........................................................................ //
  const std::string getAnyText(const std::any & x, const ValueTypeID & T) {
    switch(T) {
      case ValueTypeID::String      : return                       std::any_cast<            std::string >(x) ;
      case ValueTypeID::Integer     : return std::to_string       (std::any_cast<            int         >(x));
      case ValueTypeID::Real        : return std::to_string       (std::any_cast<            double      >(x));
      case ValueTypeID::Boolean     : return                      (std::any_cast<            bool        >(x)) ? "true" : "false";
      case ValueTypeID::StringList  : return BCG::vector_to_string(std::any_cast<std::vector<std::string>>(x));
      case ValueTypeID::IntegerList : return BCG::vector_to_string(std::any_cast<std::vector<int>>(x));
      case ValueTypeID::RealList    : return BCG::vector_to_string(std::any_cast<std::vector<double>>(x));
      case ValueTypeID::BooleanList : {
        std::string reVal;
        for (auto bit : std::any_cast<std::vector<bool>>(x)) {reVal += (bit ? "+" : "_");}
        return reVal;
      }
      default                     : return "(invalid state)";
    }
  }
  // ........................................................................ //
  const std::string getAnyText(const std::any & x) {
    if      ( !std::strcmp(x.type().name(), "NSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE"                  ) )
      {return getAnyText(x, ValueTypeID::String);}
    else if ( !std::strcmp(x.type().name(), "i"                                                                     ) )
      {return getAnyText(x, ValueTypeID::Integer);}
    else if ( !std::strcmp(x.type().name(), "d"                                                                     ) )
      {return getAnyText(x, ValueTypeID::Real);}
    else if ( !std::strcmp(x.type().name(), "b"                                                                     ) )
      {return getAnyText(x, ValueTypeID::Boolean);}
    else if ( !std::strcmp(x.type().name(), "St6vectorINSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESaIS5_EE") )
      {return getAnyText(x, ValueTypeID::StringList);}
    else if ( !std::strcmp(x.type().name(), "St6vectorIiSaIiEE"                                                     ) )
      {return getAnyText(x, ValueTypeID::IntegerList);}
    else if ( !std::strcmp(x.type().name(), "St6vectorIdSaIdEE"                                                     ) )
      {return getAnyText(x, ValueTypeID::RealList);}
    else if ( !std::strcmp(x.type().name(), "St6vectorIbSaIbEE"                                                     ) )
      {return getAnyText(x, ValueTypeID::BooleanList);}
    else
      {return "(invalid type)";}

  }
}
