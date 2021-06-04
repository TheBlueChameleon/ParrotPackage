// ========================================================================= //
// dependencies

// STL
#include <vector>
#include <string>

// own
#include "Settings/Definitions.hpp"

// ========================================================================== //
// local macro

// ========================================================================== //
// namespace

namespace Settings {

  // ======================================================================== //
  // constants

  const std::vector<std::string> defaultBooleanTextTrue  = {"TRUE", "YES", "ON"};
  const std::vector<std::string> defaultBooleanTextFalse = {"FALSE", "NO", "OFF"};
  // ------------------------------------------------------------------------ //
  const std::string valueTypeName(ValueType T) {
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
  const std::string restrictionTypeName(RestrictionType T) {
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
  const std::string restrictionViolationPolicyName(RestrictionViolationPolicy T) {
    switch (T) {
      case RestrictionViolationPolicy::Exception : return "throw a RestrictionViolationError";
      case RestrictionViolationPolicy::Warning   : return "utter a warning via stderr";
      default                                    : return "(invalid state)";
    }
  }
}
