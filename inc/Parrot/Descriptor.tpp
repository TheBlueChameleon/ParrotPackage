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
// CTor

template <typename T>
Parrot::Descriptor::Descriptor(
  std::string K,
  const T &   defaultValue,
  bool        M
) :
  key      (K),
  mandatory(M)
{
  BCG::trim(key);
  this->setValue(defaultValue);
}
// ........................................................................ //
template <typename T>
Parrot::Descriptor::Descriptor(
  std::string                       K,
  const std::initializer_list<T> &  defaultValue,
  bool                              M
) :
  key      (K),
  mandatory(M)
{
  BCG::trim(key);
  setValue(defaultValue);
}

// ======================================================================== //
// Setters

template<typename T>
void Parrot::Descriptor::setValue(const T & newVal, bool resetMetaData) {
  valueTypeID = valueTypeIDOf(newVal);
  value = newVal;
  rectify();

  resetParsing();
  if (resetMetaData) {this->resetMetaData();}
}
// ........................................................................ //
template<typename T>
void Parrot::Descriptor::setValue(const std::initializer_list<T> & list, bool resetMetaData) {setValue(std::vector<T>(list), resetMetaData);}

// ------------------------------------------------------------------------ //

template<typename T>
void Parrot::Descriptor::makeRanged(const std::string &                                                       K,
                            const T &                                                                         defaultValue,
                            PARROT_TYPE(ValueTypeID::Real)                                                    min,
                            PARROT_TYPE(ValueTypeID::Real)                                                    max,
                            RestrictionViolationPolicy                                                        policy,
                            const std::string &                                                               restrictionViolationText,
                            bool                                                                              M
) {
  reset();
  setKey(K);
  setValue(defaultValue);
  addRestriction( Restriction(min, max, policy, restrictionViolationText) );
  setMandatory(M);
}
// ........................................................................ //
template <typename T>
void Parrot::Descriptor::makeListboundPreParse(const std::string &                                            K,
                                       const T &                                                              defaultValue,
                                       const PARROT_TYPE(ValueTypeID::StringList) &                           list,
                                       bool                                                                   forbiddenList,
                                       RestrictionViolationPolicy                                             policy,
                                       const std::string &                                                    restrictionViolationText,
                                       bool                                                                   M
) {
  reset();
  setKey(K);
  setValue(defaultValue);
  setMandatory(M);

  if (
    valueTypeID == ValueTypeID::Boolean     ||
    valueTypeID == ValueTypeID::BooleanList
  ) {
    throw Parrot::RestrictionTypeError(THROWTEXT(
      "    Type "s + valueTypeName(valueTypeID) + " not compatible with list restriction!"
    ));
  }

  auto rst = Restriction(policy, restrictionViolationText);
  rst.setPreParseValidationList(list, forbiddenList);
  addRestriction(rst);
}
// ........................................................................ //
template <typename LT>
void Parrot::Descriptor::makeListboundAftParse(const std::string &                                            K,
                                       ValueTypeID                                                            T,
                                       const std::vector<LT> &                                                list,
                                       bool                                                                   forbiddenList,
                                       RestrictionViolationPolicy                                             policy,
                                       const std::string &                                                    restrictionViolationText,
                                       bool                                                                   M
) {
  if (
    T == ValueTypeID::Boolean     ||
    T == ValueTypeID::BooleanList
  ) {
    throw Parrot::RestrictionTypeError(THROWTEXT(
      "    Type "s + valueTypeName(T) + " not compatible with list restriction!"
    ));
  }

  if ( !isTypeCompatibleWithValidityList(T, valueTypeIDOf(list)) ) {
    Parrot::RestrictionTypeError(THROWTEXT(
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
void Parrot::Descriptor::makeListboundAftParse(const std::string &                                            K,
                                       const DT &                                                             defaultValue,
                                       const std::vector<LT> &                                                list,
                                       bool                                                                   forbiddenList,
                                       RestrictionViolationPolicy                                             policy,
                                       const std::string &                                                    restrictionViolationText,
                                       bool                                                                   M
) {
  auto dt = valueTypeIDOf(defaultValue);

  if (
    dt == ValueTypeID::Boolean     ||
    dt == ValueTypeID::BooleanList
  ) {
    throw Parrot::RestrictionTypeError(THROWTEXT(
      "    Type "s + valueTypeName(dt) + " not compatible with list restriction!"
    ));
  }

  if ( !isTypeCompatibleWithValidityList(dt, valueTypeIDOf(list)) ) {
    throw Parrot::RestrictionTypeError(THROWTEXT(
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
void Parrot::Descriptor::makeUserboundPreParse(const std::string &                                            K,
                                       const T &                                                              defaultValue,
                                       const std::function<bool (const PARROT_TYPE(ValueTypeID::String) &)> & uFunc,
                                       RestrictionViolationPolicy                                             policy,
                                       const std::string &                                                    restrictionViolationText,
                                       bool                                                                   M
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
void Parrot::Descriptor::makeUserboundAftParse(const std::string &                                            K,
                                       ValueTypeID                                                            T,
                                       const std::function<bool (const AT &)> &                               uFunc,
                                       RestrictionViolationPolicy                                             policy,
                                       const std::string &                                                    restrictionViolationText,
                                       bool                                                                   M
) {
  if ( T != valueTypeIDOf( AT() ) ) {
    throw Parrot::RestrictionTypeError(THROWTEXT(
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
void Parrot::Descriptor::makeUserboundAftParse(const std::string &                                            K,
                                       const T &                                                              defaultValue,
                                       const std::function<bool (const T &)> &                                uFunc,
                                       RestrictionViolationPolicy                                             policy,
                                       const std::string &                                                    restrictionViolationText,
                                       bool                                                                   M
) {
  reset();
  setKey(K);
  setValue(defaultValue);
  setMandatory(M);

  auto rst = Restriction(policy, restrictionViolationText);
  rst.setAftParseValidationFunction(uFunc);
  addRestriction(rst);

}

// ========================================================================== //

#undef THROWTEXT
