/* TODO: File descrption
 * 
 */

#ifndef SETTINGS_DEFINITIONS_HPP
#define SETTINGS_DEFINITIONS_HPP

// ========================================================================== //
// dependencies

// STL
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
  
  // ======================================================================== //
  // constants
  
  extern const std::vector<std::string> defaultBooleanTextTrue;
  extern const std::vector<std::string> defaultBooleanTextFalse;
  
  extern const std::vector<std::string> valueTypeNames;
  extern const std::vector<std::string> restrictionTypeNames;

}
// ========================================================================== //
#endif
