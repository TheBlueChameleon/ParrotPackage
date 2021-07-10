// ========================================================================= //
// dependencies

// STL
#include <stdexcept>

#include <sstream>

#include <string>
using namespace std::string_literals;


// own
#include "BCG.hpp"
#include "Parrot/Definitions.hpp"
#include "Parrot/Descriptor.hpp"

using namespace Parrot;

// ========================================================================== //
// local macro

#define THROWTEXT(msg) ("RUNTIME EXCEPTION IN "s + (__PRETTY_FUNCTION__) + "\n"s + msg)

// ========================================================================== //
// Rectifyers

void Descriptor::rectify() {
  switch (valueTypeID) {
    case ValueTypeID::String :
      try                                 {value =              std::any_cast<std::string >(value)  ;}
      catch (const std::bad_any_cast & e) {value = std::string( std::any_cast<const char *>(value) );}
      break;

    case ValueTypeID::Integer :
      value = std::any_cast<PARROT_TYPE(ValueTypeID::Integer)>(value);
      break;

    case ValueTypeID::Real :
      value = value;
      break;
    case ValueTypeID::Boolean :
      value = value;
      break;
    case ValueTypeID::StringList :
      try {value = std::any_cast< std::vector<std::string> >(value);}
      catch (const std::bad_any_cast & e) {
        const auto & old = std::any_cast< std::vector<char const*> >(value);
        value = std::vector<std::string>(old.begin(), old.end());
      }
      break;

    case ValueTypeID::IntegerList :
      value = value;
      break;
    case ValueTypeID::RealList :
      value = value;
      break;
    case ValueTypeID::BooleanList :
      value = value;
      break;
    default:
      throw std::runtime_error(THROWTEXT("    invalid data type encountered."));
      break;

   }
}
// -------------------------------------------------------------------------- //
void Descriptor::rectify_Integer    (PARROT_TYPE(ValueTypeID::Integer) newVal) {value = static_cast<PARROT_TYPE(ValueTypeID::Integer)>(newVal);}
// .......................................................................... //
void Descriptor::rectify_Real       (PARROT_TYPE(ValueTypeID::Real   ) newVal) {value = static_cast<PARROT_TYPE(ValueTypeID::Real   )>(newVal);}
// .......................................................................... //
void Descriptor::rectify_Boolean    (PARROT_TYPE(ValueTypeID::Boolean) newVal) {value = static_cast<PARROT_TYPE(ValueTypeID::Boolean)>(newVal);}
// .......................................................................... //
void Descriptor::rectify_IntegerList(PARROT_TYPE(ValueTypeID::IntegerList) newVal) {value = static_cast<PARROT_TYPE(ValueTypeID::IntegerList)>(newVal);}
// .......................................................................... //
void Descriptor::rectify_RealList   (PARROT_TYPE(ValueTypeID::RealList   ) newVal) {value = static_cast<PARROT_TYPE(ValueTypeID::RealList   )>(newVal);}
// .......................................................................... //
void Descriptor::rectify_BooleanList(PARROT_TYPE(ValueTypeID::BooleanList) newVal) {value = static_cast<PARROT_TYPE(ValueTypeID::BooleanList)>(newVal);}

// ========================================================================== //
// CTor, DTor

Descriptor::Descriptor(
  std::string K,
  ValueTypeID T,
  bool        M
) :
  key        (K),
  valueTypeID(T),
  mandatory  (M)
{}

// ========================================================================== //
// Getters

const std::string Descriptor::getKey          () const {return key;}
// .......................................................................... //
std::any          Descriptor::getValue        () const {return value;}
// .......................................................................... //
ValueTypeID       Descriptor::getValueTypeID  () const {return valueTypeID;}
// .......................................................................... //
const std::string Descriptor::getValueTypeName() const {return valueTypeName(valueTypeID);}
// .......................................................................... //
const std::string Descriptor::getTypeID       () const {return value.type().name();}
// -------------------------------------------------------------------------- //
bool              Descriptor::isKeyCaseSensitive       () const {return keyCaseSensitive;}
bool              Descriptor::isValueCaseSensitive     () const {return valueCaseSensitive;}
// -------------------------------------------------------------------------- //
bool              Descriptor::isTrimLeadingWhitespaces () const {return trimLeadingWhitespaces;}
bool              Descriptor::isTrimTrailingWhitespaces() const {return trimTrailingWhitespaces;}
// -------------------------------------------------------------------------- //
bool              Descriptor::isMandatory              () const {return mandatory;}
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
  valueTypeID = ValueTypeID::Integer;

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
void Descriptor::addRestriction (const Restriction & restriction) {


  restrictions.push_back(restriction);
}
// .......................................................................... //
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
  ValueTypeID                 T,
  double min, double max,
  RestrictionViolationPolicy  policy,
  const std::string &         restrictionViolationText,
  bool                        M
) {
  if (
    T != ValueTypeID::Integer     &&
    T != ValueTypeID::Real        &&
    T != ValueTypeID::IntegerList &&
    T != ValueTypeID::RealList
  ) {
    throw std::runtime_error(THROWTEXT(
      "    Type "s + valueTypeName(T) + " not compatible with range restriction!"
    ));
  }

  reset();
  setKey(K);
  valueTypeID = T;

  addRestriction( Restriction(min, max, policy, restrictionViolationText) );
  setMandatory(M);
}
// .......................................................................... //
void Descriptor::makeListboundPreParse(
  const std::string &               K,
  ValueTypeID                       T,
  const std::vector<std::string> &  list,
  bool                              forbiddenList,
  RestrictionViolationPolicy        policy,
  const std::string &               restrictionViolationText,
  bool                              M
) {
  if (
    T == ValueTypeID::Boolean     ||
    T == ValueTypeID::BooleanList
  ) {
    throw std::runtime_error(THROWTEXT(
      "    Type "s + valueTypeName(T) + " not compatible with list restriction!"
    ));
  }

  reset();
  setKey(K);
  valueTypeID = T;

  auto rst = Restriction(policy, restrictionViolationText);
  rst.setPreParseValidationList(list, forbiddenList);
  addRestriction(rst);

  setMandatory(M);
}
// .......................................................................... //
void Descriptor::makeUserboundPreParse(
  const std::string &                                K,
  ValueTypeID                                        T,
  const std::function<bool (const std::string &)> &  uFunc,
  RestrictionViolationPolicy                         policy,
  const std::string &                                restrictionViolationText,
  bool                                               M
) {
  reset();
  setKey(K);
  valueTypeID = T;

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

  reVal << "  Datatype                 : " << valueTypeName(valueTypeID) << "\n";
  reVal << "  Default value            : " << (value.has_value() ? getAnyText(value) : "[none]")  << "\n";

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
      auto lines = BCG::splitString(rst.to_string(), '\n');

      for (auto & line : lines) {
        reVal << "    " << line << "\n";
      }
    }
  }

  return reVal.str();
}
