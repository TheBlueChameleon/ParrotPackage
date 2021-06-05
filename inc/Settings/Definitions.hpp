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
#include <any>
#include <initializer_list>

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
  // lookups
  
  extern const std::vector<std::string> defaultBooleanTextTrue;
  extern const std::vector<std::string> defaultBooleanTextFalse;
  
  const std::string valueTypeName                 (const ValueType                  & T);
  const std::string restrictionTypeName           (const RestrictionType            & T);
  const std::string restrictionViolationPolicyName(const RestrictionViolationPolicy & T);

  // ======================================================================== //
  // type interpreters

  template<typename T>
  constexpr ValueType valueTypeOf(const T & x);

  template<typename T>
  constexpr ValueType valueTypeOf(const std::initializer_list<T> & x);

  const std::string getTypeIDOf(const std::any & x);
  template<typename T>
  const std::string getTypeIDOf(const std::initializer_list<T> & x);

  const std::string getAnyText(const std::any & x, const ValueType & T);
}
// ========================================================================== //
// template implementations

#include "Settings/Definitions.tpp"

// ========================================================================== //
#endif
