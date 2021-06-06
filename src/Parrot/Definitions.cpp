// ========================================================================= //
// dependencies

// STL
#include <iostream>

#include <vector>
#include <string>

// own
#include "globals.hpp"
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
  const std::string valueTypeName(const ValueType & T) {
    switch (T) {
      case ValueType::String      : return "string";
      case ValueType::Integer     : return "integer";
      case ValueType::Real        : return "real value";
      case ValueType::Boolean     : return "boolean";
      case ValueType::StringList  : return "list of strings";
      case ValueType::IntegerList : return "list of integers";
      case ValueType::RealList    : return "list of real values";
      case ValueType::BooleanList : return "list of booleans";
      default                     : return "(invalid state)";
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
  bool isTypeCompatibleWithValidityList(const ValueType & valType, const ValueType & listType) {
    if (
      listType != ValueType::StringList  &&
      listType != ValueType::IntegerList &&
      listType != ValueType::RealList
    ) {return false;}

    if (valType == listType)                                                 {return true;}
    if (valType == ValueType::String  && listType == ValueType::StringList ) {return true;}
    if (valType == ValueType::Integer && listType == ValueType::IntegerList) {return true;}
    if (valType == ValueType::Real    && listType == ValueType::RealList   ) {return true;}

    return false;
  }

  // ======================================================================== //
  // type interpreters

  const std::string getTypeIDOf(const std::any & x) {return x.type().name();}
  // ........................................................................ //
  const std::string getAnyText(const std::any & x, const ValueType & T) {
    switch(T) {
      case ValueType::String      : return                  std::any_cast<            std::string >(x) ;
      case ValueType::Integer     : return std::to_string  (std::any_cast<            int         >(x));
      case ValueType::Real        : return std::to_string  (std::any_cast<            double      >(x));
      case ValueType::Boolean     : return                 (std::any_cast<            bool        >(x)) ? "true" : "false";
      case ValueType::StringList  : return vector_to_string(std::any_cast<std::vector<std::string>>(x));
      case ValueType::IntegerList : return vector_to_string(std::any_cast<std::vector<int>>(x));
      case ValueType::RealList    : return vector_to_string(std::any_cast<std::vector<double>>(x));
      case ValueType::BooleanList : {
        std::string reVal;
        for (auto bit : std::any_cast<std::vector<bool>>(x)) {reVal += (bit ? "+" : "_");}
        return reVal;
      }
      default                     : return "(invalid state)";
    }
  }

}
