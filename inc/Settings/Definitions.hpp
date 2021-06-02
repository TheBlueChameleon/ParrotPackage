/* TODO: File descrption
 * 
 */

#ifndef SETTINGS_DEFINITIONS_HPP
#define SETTINGS_DEFINITIONS_HPP

// ========================================================================== //
// dependencies

// STL
#include <exception>

#include <vector>
#include <string>

// ========================================================================== //
namespace Settings {
  
  // ======================================================================== //
  // enums

  enum class ValueType {
    String,
    Integer,
    Real,
    Boolean,
    StringList,
    IntegerList,
    RealList,
    BooleanList
  };
  
  // ------------------------------------------------------------------------ //
  
  enum class RestrictionType {
    None,
    AllowedList,
    ForbiddenList,
    Range,
    Function
  };
  
  // ------------------------------------------------------------------------ //
  
  enum class RestrictionViolationPolicy {
    Exception,
    Warning
  };
  
  // ======================================================================== //
  // structs and classes
  
  class RestrictionViolationError : public std::exception {};
  
  // ======================================================================== //
  // constants
  
  extern const std::vector<std::string> defaultBooleanTextTrue;
  extern const std::vector<std::string> defaultBooleanTextFalse;
  
  extern const std::vector<std::string> valueTypeNames;
  extern const std::vector<std::string> restrictionTypeNames;
  extern const std::vector<std::string> restrictionViolationPolicyNames;

}
// ========================================================================== //
#endif
