// ========================================================================= //
// dependencies

// STL
#include <stdexcept>

#include <sstream>

#include <string>
using namespace std::string_literals;


// own
#include "globals.hpp"
#include "Settings/Definitions.hpp"
#include "Settings/Descriptor.hpp"

using namespace Settings;

// ========================================================================== //
// local macro

#define THROWTEXT(msg) ("RUNTIME EXCEPTION IN "s + (__PRETTY_FUNCTION__) + "\n"s + msg)

// ========================================================================== //
// Rectifyers

void Descriptor::rectifyText() {
  /* This function should only be called if it is known that member value
   * holds a ValueType::String or ValueType::StringList. It makes sure that all
   * string data are represented as std::string
   *
   * typeID strings follow a complex set of rules.
   * Primitive types are built from simple symbol:
   * P: Pointer; K: const; c: char
   * so, a PKc and a Pc an be used as a basis for std::strings.
   */

  auto typeIDString = value.type().name();

  if        (valueType == ValueType::String) {
    if (typeIDString[0] == 'P') {value = std::string( std::any_cast<const char *>(value) );}
  } else if (valueType == ValueType::StringList) {
    if (
      typeIDString == "St6vectorIPKcSaIS1_EE"s ||                               // std::vector<const char *>
      typeIDString == "St6vectorIPcSaIS1_EE"s                                   // std::vector<      char *>
    ) {
      auto & old = std::any_cast<const std::vector<const char *>>(value);
      std::vector<std::string> newVal(old.begin(), old.end());
      value = newVal;
    }

  }
}

// ========================================================================== //
// CTor, DTor

Descriptor::Descriptor(
  std::string K,
  ValueType T,
  bool M
) :
  key      (K),
  valueType(T),
  mandatory(M)
{}

// ========================================================================== //
// Getters

const std::string Descriptor::getKey          () const {return key;}
std::any          Descriptor::getValue        () const {return value;}
ValueType         Descriptor::getValueType    () const {return valueType;}
const std::string Descriptor::getValueTypeName() const {return valueTypeName(valueType);}
const std::string Descriptor::getTypeID       () const {return value.type().name();}
// -------------------------------------------------------------------------- //
bool          Descriptor::isKeyCaseSensitive       () const {return keyCaseSensitive;}
bool          Descriptor::isValueCaseSensitive     () const {return valueCaseSensitive;}
// -------------------------------------------------------------------------- //
bool          Descriptor::isTrimLeadingWhitespaces () const {return trimLeadingWhitespaces;}
bool          Descriptor::isTrimTrailingWhitespaces() const {return trimTrailingWhitespaces;}
// -------------------------------------------------------------------------- //
bool          Descriptor::isMandatory              () const {return mandatory;}
// -------------------------------------------------------------------------- //
const std::vector<Restriction>                                  & Descriptor::getRestrictions () const {return restrictions;}
// -------------------------------------------------------------------------- //
const std::vector<std::pair<std::string, std::string>>          & Descriptor::getSubstitutions() const {return substitutions;}
const std::function<const std::string & (const std::string &)>  & Descriptor::getUserPreParser() const {return userPreParser;}

// ========================================================================== //
// Setters

void Descriptor::reset () {
  key = "";
  value.reset();
  valueType = ValueType::Integer;

  keyCaseSensitive        = false;
  valueCaseSensitive      = false;

  trimLeadingWhitespaces  = true; 
  trimTrailingWhitespaces = true;

  mandatory               = false;
  
  restrictions.clear();
  substitutions.clear();
  userPreParser = nullptr;
}
// -------------------------------------------------------------------------- //
void Descriptor::setKey (const std::string & newVal) {key = newVal;}
// -------------------------------------------------------------------------- //
void Descriptor::setKeyCaseSensitive        (bool newVal) {  keyCaseSensitive = newVal;}
void Descriptor::setValueCaseSensitive      (bool newVal) {valueCaseSensitive = newVal;}
// .......................................................................... //
void Descriptor::setTrimLeadingWhitespaces  (bool newVal) {trimLeadingWhitespaces  = newVal;}
void Descriptor::setTrimTrailingWhitespaces (bool newVal) {trimTrailingWhitespaces = newVal;}
// .......................................................................... //
void Descriptor::setMandatory               (bool newVal) {mandatory = newVal;}
// -------------------------------------------------------------------------- //
void Descriptor::addRestriction (const Restriction & restriction) {restrictions.push_back(restriction);}
void Descriptor::clearRestrictions () {restrictions.clear();}
// .......................................................................... //
void Descriptor::addSubstitution (const std::string & substituee, const std::string & substitute) {
  substitutions.push_back(
    std::make_pair<std::string, std::string>(substituee.data(), substitute.data())
    // make_pair requires lvalue references. Using .data forces the compiler to construct a copy from the string
  );
}
void Descriptor::clearSubstitutions () {substitutions.clear();}
// .......................................................................... //
void Descriptor::setUserPreParser(const std::function<const std::string & (const std::string &)> & uFunc) {userPreParser = uFunc;}
void Descriptor::clearUserPreParser() {userPreParser = nullptr;}
// -------------------------------------------------------------------------- //
void Descriptor::makeRanged(
  const std::string &         K,
  ValueType                   T,
  double min, double max,
  RestrictionViolationPolicy  policy,
  const std::string &         restrictionViolationText,
  bool                        M
) {
  if (
    T != ValueType::Integer     &&
    T != ValueType::Real        &&
    T != ValueType::IntegerList &&
    T != ValueType::RealList
  ) {
    throw std::runtime_error(THROWTEXT(
      "    Type "s + valueTypeName(T) + " not compatible with range restriction!"
    ));
  }

  reset();
  setKey(K);
  valueType = T;

  addRestriction( Restriction(min, max, policy, restrictionViolationText) );
  setMandatory(M);
}
// .......................................................................... //
void Descriptor::makeListboundPreParse(
  const std::string &               K,
  ValueType                         T,
  const std::vector<std::string> &  list,
  bool                              forbiddenList,
  RestrictionViolationPolicy        policy,
  const std::string &               restrictionViolationText,
  bool                              M
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
  valueType = T;

  auto rst = Restriction(policy, restrictionViolationText);
  rst.setPreParseValidationList(list, forbiddenList);
  addRestriction(rst);

  setMandatory(M);
}
// .......................................................................... //
void Descriptor::makeUserboundPreParse(
  const std::string &                                K,
  ValueType                                          T,
  const std::function<bool (const std::string &)> &  uFunc,
  RestrictionViolationPolicy                         policy,
  const std::string &                                restrictionViolationText,
  bool                                               M
) {
  reset();
  setKey(K);
  valueType = T;

  auto rst = Restriction(policy, restrictionViolationText);
  rst.setPreParseValidationFunction(uFunc);
  addRestriction(rst);

  setMandatory(M);
}

// ========================================================================== //
// Representation

std::string Descriptor::to_string() const {
  std::ostringstream reVal;

  reVal << "Descriptor";
  if ( key.empty() )  {reVal << " (uninitialized keyword)\n";}
  else                {reVal << "for keyowrd '" << key << "'\n";}

  reVal << "  Datatype                 : " << valueTypeName(valueType) << "\n";
  reVal << "  Default value            : " << (value.has_value() ? getAnyText(value, valueType) : "[none]")  << "\n";

  reVal << std::boolalpha;
  reVal << "  Keyword case sensitive   : " << keyCaseSensitive        << "\n";
  reVal << "  Value case sensitive     : " << valueCaseSensitive      << "\n";
  reVal << "  Trim leading whitespaces : " << trimLeadingWhitespaces  << "\n";
  reVal << "  Trim trailing whitespaces: " << trimTrailingWhitespaces << "\n";
  reVal << "  Keyword mandatory        : " << mandatory               << "\n";

  if ( userPreParser ) {reVal << "  with userdefined preparsing function" << "\n";}

  if ( !substitutions.empty() ) {
    reVal << "  with substitutions:" << "\n";
    for (auto & sub : substitutions) {
        reVal << "    " << "'" << sub.first << "' into '" << sub.second << "'" << "\n";
    }
  }

  if ( !restrictions.empty() ) {
    reVal << "  with restrictions:" << "\n";
    for (auto & rst : restrictions) {
      auto lines = splitString(rst.to_string(), '\n');
      for (auto & line : lines) {
        reVal << "    " << line << "\n";
      }
    }
  }

  return reVal.str();
}
