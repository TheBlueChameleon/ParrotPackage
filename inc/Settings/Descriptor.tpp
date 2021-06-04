// ========================================================================= //
// dependencies
// STL
#include <stdexcept>

#include <vector>
#include <any>
#include <initializer_list>

// own
#include "Settings/Definitions.hpp"

// ========================================================================== //
// local macro

#define THROWTEXT(msg) ("RUNTIME EXCEPTION IN "s + (__PRETTY_FUNCTION__) + "\n"s + msg)

// ========================================================================== //
namespace Settings {
  // ======================================================================== //
  // Setters

  template<typename T>
  void Descriptor::setValue(T newVal, bool resetMetaData) {
    if      ( std::is_same<bool,                              T>::value ) {valueType = ValueType::Boolean    ;}
    else if ( std::is_integral<                               T>::value ) {valueType = ValueType::Integer    ;}
    else if ( std::is_floating_point<                         T>::value ) {valueType = ValueType::Real       ;}
    else if ( std::is_constructible<std::string,              T>::value ) {valueType = ValueType::String     ;}
    else if ( std::is_constructible<std::vector<std::string>, T>::value ) {valueType = ValueType::StringList ;}
    else if ( std::is_constructible<std::vector<bool>,        T>::value ) {valueType = ValueType::BooleanList;}
    else if ( std::is_constructible<std::vector<int>,         T>::value ) {valueType = ValueType::IntegerList;}
    else if ( std::is_constructible<std::vector<double>,      T>::value ) {valueType = ValueType::RealList   ;}
    else {throw std::invalid_argument(std::string(__PRETTY_FUNCTION__) + "\nType not supported.");}
    
    value = newVal;
    
    if (resetMetaData) {
      restrictions.clear();
      substitutions.clear();
      userPreParser = nullptr;
    }
  }
  // ........................................................................ //
  template<typename T>
  void Descriptor::setValue(const std::initializer_list<T> & list, bool resetMetaData) {setValue(std::vector<T>(list), resetMetaData);}
  
  // ------------------------------------------------------------------------ //
  
  template <typename DT>
  void Descriptor::makeListboundAftParse(
    const std::string &         K,
    const std::vector<DT> &     list,
    bool                        forbiddenList,
    ValueType                   T,
    const std::any &            defaultValue,
    RestrictionViolationPolicy  policy,
    const std::string &         restrictionViolationText,
    bool                        M
  ) {
    if (
      T == ValueType::Boolean     ||
      T == ValueType::BooleanList
    ) {
      throw std::runtime_error(THROWTEXT(
        "    Type "s + valueTypeName(T) + " not compatible with list restriction!"
      ));
    }
    
    reset();
    setKey(K);
    
    if ( defaultValue.has_value() ) {
      setValue(defaultValue);
      if ( valueType != T ) {
        throw std::runtime_error(THROWTEXT(
          "    Type "s + valueTypeName(T) + " does not match default value type (" + valueTypeName(valueType) + ")"
        ));
      }
    }
    
    auto rst = Restriction(policy, restrictionViolationText);
    rst.setAftParseList(list, forbiddenList);
    addRestriction(rst);
    
    setMandatory(M);
  }
  // ........................................................................ //
  template <typename DT>
  void Descriptor::makeUserboundAftParse(
  const std::string &                       K,
  const std::function<bool (const DT &)> &  uFunc,
  ValueType                                 T,
  const std::any &                          defaultValue,
  RestrictionViolationPolicy                policy,
  const std::string &                       restrictionViolationText,
  bool                                      M
  ) {
    reset();
    setKey(K);
    
    if ( defaultValue.has_value() ) {
      setValue(defaultValue);
      if ( valueType != T ) {
        throw std::runtime_error(THROWTEXT(
          "    Type "s + valueTypeName(T) + " does not match default value type (" + valueTypeName(valueType) + ")"
        ));
      }
    }
    
    auto rst = Restriction(policy, restrictionViolationText);
    rst.setAftParseFunction(uFunc);
    addRestriction(rst);
    
    setMandatory(M);
  }
}
// ========================================================================== //
#undef THROWTEXT
