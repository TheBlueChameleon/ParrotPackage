// ========================================================================= //
// dependencies
// STL
#include <stdexcept>

#include <vector>
#include <any>
#include <initializer_list>

// own
#include "Parrot/Definitions.hpp"

// ========================================================================== //
// local macro

#define THROWTEXT(msg) ("RUNTIME EXCEPTION IN "s + (__PRETTY_FUNCTION__) + "\n"s + msg)

// ========================================================================== //

namespace Parrot {
  // ======================================================================== //
  // Rectifyers

  template<class T>
  void Descriptor::rectify_String     (T newVal) {
    try                                 {value =              std::any_cast<std::string >(newVal)  ;}
    catch (const std::bad_any_cast & e) {value = std::string( std::any_cast<const char *>(newVal) );}
  }
  // ........................................................................ //

  template<class T>
  void Descriptor::rectify_StringList (T newVal) {
    try {value = std::any_cast< std::vector<std::string> >(newVal);}
    catch (const std::bad_any_cast & e) {
      const auto & old = std::any_cast< std::vector<char const*> >(newVal);
      value = std::vector<std::string>(old.begin(), old.end());
    }
  }

  // ======================================================================== //
  // CTor

  template <typename T>
  Descriptor::Descriptor(
    std::string K,
    const T & defaultValue,
    bool M
  ) :
    key      (K),
    mandatory(M)
  {
    setValue(defaultValue);
  }
  // ........................................................................ //
  template <typename T>
  Descriptor::Descriptor(
    std::string K,
    const std::initializer_list<T> & defaultValue,
    bool M
  ) :
    key      (K),
    mandatory(M)
  {
    setValue(defaultValue);
  }

  // ======================================================================== //
  // Setters

  template<typename T>
  void Descriptor::setValue(const T & newVal, bool resetMetaData) {
    valueTypeID = valueTypeIDOf(newVal);
    value = newVal;

    rectify();

//     switch (valueTypeID) {
//       case ValueTypeID::String :
//         rectify_String(newVal);
//
//         break;
//
//       case ValueTypeID::Integer :
//         rectify_Integer(newVal);
//         break;
//
//       case ValueTypeID::Real :
//         value = value;
//         break;
//       case ValueTypeID::Boolean :
//         value = value;
//         break;
//       case ValueTypeID::StringList :
//         try {value = std::any_cast< std::vector<std::string> >(value);}
//         catch (const std::bad_any_cast & e) {
//           const auto & old = std::any_cast< std::vector<char const*> >(value);
//           value = std::vector<std::string>(old.begin(), old.end());
//         }
//         break;
//
//       case ValueTypeID::IntegerList :
//         value = value;
//         break;
//       case ValueTypeID::RealList :
//         value = value;
//         break;
//       case ValueTypeID::BooleanList :
//         value = value;
//         break;
//     }


    if (resetMetaData) {
      restrictions .clear();
      substitutions.clear();
      userPreParser           = nullptr;
      keyCaseSensitive        = false;
      valueCaseSensitive      = false;
      trimLeadingWhitespaces  = true;
      trimTrailingWhitespaces = true;
      mandatory               = false;
    }
  }
  // ........................................................................ //
  template<typename T>
  void Descriptor::setValue(const std::initializer_list<T> & list, bool resetMetaData) {setValue(std::vector<T>(list), resetMetaData);}
  
  // ------------------------------------------------------------------------ //
  
  template<typename T>
  void Descriptor::makeRanged(
    const std::string &         K,
    const T &                   defaultValue,
    double min, double max,
    RestrictionViolationPolicy  policy,
    const std::string &         restrictionViolationText,
    bool                        M
  ) {
    reset();
    setKey(K);
    setValue(defaultValue);
    setMandatory(M);

    if (
      valueTypeID != ValueTypeID::Integer     &&
      valueTypeID != ValueTypeID::Real        &&
      valueTypeID != ValueTypeID::IntegerList &&
      valueTypeID != ValueTypeID::RealList
    ) {
      throw std::runtime_error(THROWTEXT(
        "    Type " + valueTypeName(valueTypeID) + " not compatible with range restriction!"
      ));
    }

    addRestriction( Restriction(min, max, policy, restrictionViolationText) );
  }
  // ........................................................................ //
  template <typename T>
  void Descriptor::makeListboundPreParse(
    const std::string &                               K,
    const T &                                         defaultValue,
    const std::vector<std::string> &                  list,
    bool                                              forbiddenList,
    RestrictionViolationPolicy                        policy,
    const std::string &                               restrictionViolationText,
    bool                                              M
  ) {
    reset();
    setKey(K);
    setValue(defaultValue);
    setMandatory(M);

    if (
      valueTypeID == ValueTypeID::Boolean     ||
      valueTypeID == ValueTypeID::BooleanList
    ) {
      throw std::runtime_error(THROWTEXT(
        "    Type "s + valueTypeName(valueTypeID) + " not compatible with list restriction!"
      ));
    }

    auto rst = Restriction(policy, restrictionViolationText);
    rst.setPreParseValidationList(list, forbiddenList);
    addRestriction(rst);
  }
  // ........................................................................ //
  template <typename LT>
  void Descriptor::makeListboundAftParse(
    const std::string &         K,
    ValueTypeID                 T,
    const std::vector<LT> &     list,
    bool                        forbiddenList,
    RestrictionViolationPolicy  policy,
    const std::string &         restrictionViolationText,
    bool                        M
  ) {
    if (
      T == ValueTypeID::Boolean     ||
      T == ValueTypeID::BooleanList
    ) {
      throw std::runtime_error(THROWTEXT(
        "    Type "s + valueTypeName(T) + " not compatible with list restriction!"
      ));
    }
    
    if ( !isTypeCompatibleWithValidityList(T, valueTypeIDOf(list)) ) {
      throw std::runtime_error(THROWTEXT(
        "    Type "s + valueTypeName(T) + " not compatible type of list (" + valueTypeName(valueTypeIDOf(list)) + ")"
      ));
    }

    reset();
    setKey(K);
    valueTypeID = T;
    setMandatory(M);

    auto rst = Restriction(policy, restrictionViolationText);
    rst.setAftParseValidationList(list, forbiddenList);
    addRestriction(rst);
  }
  // ........................................................................ //
  template <typename DT, typename LT>
  void Descriptor::makeListboundAftParse(
    const std::string &         K,
    const DT &                  defaultValue,
    const std::vector<LT> &     list,
    bool                        forbiddenList,
    RestrictionViolationPolicy  policy,
    const std::string &         restrictionViolationText,
    bool                        M
  ) {
    auto dt = valueTypeIDOf(defaultValue);

    if (
      dt == ValueTypeID::Boolean     ||
      dt == ValueTypeID::BooleanList
    ) {
      throw std::runtime_error(THROWTEXT(
        "    Type "s + valueTypeName(dt) + " not compatible with list restriction!"
      ));
    }

    if ( !isTypeCompatibleWithValidityList(dt, valueTypeIDOf(list)) ) {
      throw std::runtime_error(THROWTEXT(
        "    Type "s + valueTypeName(dt) + " not compatible type of list (" + valueTypeName(valueTypeIDOf(list)) + ")"
      ));
    }

    reset();
    setKey(K);
    setValue(defaultValue);
    setMandatory(M);

    auto rst = Restriction(policy, restrictionViolationText);
    rst.setAftParseValidationList(list, forbiddenList);
    addRestriction(rst);
  }
  // ........................................................................ //
  template <typename T>
  void Descriptor::makeUserboundPreParse(
    const std::string &                                K,
    const T &                                          defaultValue,
    const std::function<bool (const std::string &)> &  uFunc,
    RestrictionViolationPolicy                         policy,
    const std::string &                                restrictionViolationText,
    bool                                               M
  ) {
    reset();
    setKey(K);
    setValue(defaultValue);
    setMandatory(M);

    auto rst = Restriction(policy, restrictionViolationText);
    rst.setPreParseValidationFunction(uFunc);
    addRestriction(rst);
  }
  // ........................................................................ //
  template <typename AT>
  void Descriptor::makeUserboundAftParse(
    const std::string &                       K,
    ValueTypeID                                 T,
    const std::function<bool (const AT &)> &  uFunc,
    RestrictionViolationPolicy                policy,
    const std::string &                       restrictionViolationText,
    bool                                      M
  ) {

    if ( T != valueTypeIDOf( AT() ) ) {
      throw std::runtime_error(THROWTEXT(
        "    Type "s + valueTypeName(T) + " not compatible type of user validation argument (" + valueTypeName(valueTypeIDOf( AT() )) + ")"
      ));
    }

    reset();
    setKey(K);
    valueTypeID = T;
    setMandatory(M);

    auto rst = Restriction(policy, restrictionViolationText);
    rst.setAftParseValidationFunction(uFunc);
    addRestriction(rst);

  }
  // ........................................................................ //
  template <typename T>
  void Descriptor::makeUserboundAftParse(
  const std::string &                      K,
  const T &                                defaultValue,
  const std::function<bool (const T &)> &  uFunc,
  RestrictionViolationPolicy               policy,
  const std::string &                      restrictionViolationText,
  bool                                     M
  ) {
    reset();
    setKey(K);
    setValue(defaultValue);
    setMandatory(M);

    auto rst = Restriction(policy, restrictionViolationText);
    rst.setAftParseValidationFunction(uFunc);
    addRestriction(rst);

  }
}

// ========================================================================== //

#undef THROWTEXT
