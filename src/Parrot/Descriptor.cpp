// ========================================================================= //
// dependencies

// STL
#include <stdexcept>

#include <sstream>

#include <string>
using namespace std::string_literals;

#include <algorithm>

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
    case ValueTypeID::None        :                        break;
    case ValueTypeID::String      : rectify_String     (); break;
    case ValueTypeID::Integer     : rectify_Integer    (); break;
    case ValueTypeID::Real        : rectify_Real       (); break;
    case ValueTypeID::Boolean     : rectify_Boolean    (); break;
    case ValueTypeID::StringList  : rectify_StringList (); break;
    case ValueTypeID::IntegerList : rectify_IntegerList(); break;
    case ValueTypeID::RealList    : rectify_RealList   (); break;
    case ValueTypeID::BooleanList : rectify_BooleanList(); break;
   }
}
// -------------------------------------------------------------------------- //
void Descriptor::rectify_String     () {
  try                                 {value =              std::any_cast<std::string >(value)  ;}
  catch (const std::bad_any_cast & e) {value = std::string( std::any_cast<const char *>(value) );}
}
// .......................................................................... //
void Descriptor::rectify_Integer    () {
  PARROT_TYPE(ValueTypeID::Integer) newVal;

  auto type = BCG::demangle( value.type().name() );

  if      (type == "char"              ) {newVal = std::any_cast<char              >(value);}
  else if (type == "short"             ) {newVal = std::any_cast<short             >(value);}
  else if (type == "int"               ) {newVal = std::any_cast<int               >(value);}
  else if (type == "long"              ) {newVal = std::any_cast<long              >(value);}
  else if (type == "long long"         ) {newVal = std::any_cast<long long         >(value);}

  else if (type == "unsigned char"     ) {newVal = std::any_cast<unsigned char     >(value);}
  else if (type == "unsigned short"    ) {newVal = std::any_cast<unsigned short    >(value);}
  else if (type == "unsigned int"      ) {newVal = std::any_cast<unsigned int      >(value);}
  else if (type == "unsigned long"     ) {newVal = std::any_cast<unsigned long     >(value);}
  else if (type == "unsigned long long") {newVal = std::any_cast<unsigned long long>(value);}

  else                                   {newVal = 0;}

  value = newVal;
}
// .......................................................................... //
void Descriptor::rectify_Real       () {
  PARROT_TYPE(ValueTypeID::Real) newVal;

  auto type = BCG::demangle( value.type().name() );

  if      (type == "float"      ) {newVal = std::any_cast<float      >(value);}
  else if (type == "double"     ) {newVal = std::any_cast<double     >(value);}
  else if (type == "long double") {newVal = std::any_cast<long double>(value);}
  else                            {newVal = 0.;}

  value = newVal;
}
// .......................................................................... //
void Descriptor::rectify_Boolean    () {value = std::any_cast<bool>(value);}
// .......................................................................... //
void Descriptor::rectify_StringList () {
  try {value = std::any_cast< std::vector<std::string> >(value);}
  catch (const std::bad_any_cast & e) {
    const auto & old = std::any_cast< std::vector<char const *> >(value);
    value = std::vector<std::string>(old.begin(), old.end());
  }
}
// .......................................................................... //
void Descriptor::rectify_IntegerList() {
  PARROT_TYPE(ValueTypeID::IntegerList) newVal;

  auto type = BCG::demangle( value.type().name() );

  if        (type == "std::vector<char, std::allocator<char> >"                            ) {
    auto src = std::any_cast< std::vector<char> >(value);
    newVal.reserve(src.size());
    std::transform(src.begin(), src.end(),
                   std::back_inserter(newVal),
                   [] (auto & v) {return PARROT_TYPE(ValueTypeID::Integer)(v);}
                   );

  } else if (type == "std::vector<short, std::allocator<short> >"                          ) {
    auto src = std::any_cast< std::vector<short> >(value);
    newVal.reserve(src.size());
    std::transform(src.begin(), src.end(),
                   std::back_inserter(newVal),
                   [] (auto & v) {return PARROT_TYPE(ValueTypeID::Integer)(v);}
                   );

  } else if (type == "std::vector<int, std::allocator<int> >"                              ) {
    auto src = std::any_cast< std::vector<int> >(value);
    newVal.reserve(src.size());
    std::transform(src.begin(), src.end(),
                   std::back_inserter(newVal),
                   [] (auto & v) {return PARROT_TYPE(ValueTypeID::Integer)(v);}
                   );

  } else if (type == "std::vector<long, std::allocator<long> >"                            ) {
    auto src = std::any_cast< std::vector<long> >(value);
    newVal.reserve(src.size());
    std::transform(src.begin(), src.end(),
                   std::back_inserter(newVal),
                   [] (auto & v) {return PARROT_TYPE(ValueTypeID::Integer)(v);}
                   );

  } else if (type == "std::vector<long long, std::allocator<long long> >"                  ) {
    auto src = std::any_cast< std::vector<long long> >(value);
    newVal.reserve(src.size());
    std::transform(src.begin(), src.end(),
                   std::back_inserter(newVal),
                   [] (auto & v) {return PARROT_TYPE(ValueTypeID::Integer)(v);}
                   );

  } else if (type == "std::vector<unsigned char, std::allocator<unsigned char> >"          ) {
    auto src = std::any_cast< std::vector<unsigned char> >(value);
    newVal.reserve(src.size());
    std::transform(src.begin(), src.end(),
                   std::back_inserter(newVal),
                   [] (auto & v) {return PARROT_TYPE(ValueTypeID::Integer)(v);}
                   );

  } else if (type == "std::vector<unsigned short, std::allocator<unsigned short> >"        ) {
    auto src = std::any_cast< std::vector<unsigned short> >(value);
    newVal.reserve(src.size());
    std::transform(src.begin(), src.end(),
                   std::back_inserter(newVal),
                   [] (auto & v) {return PARROT_TYPE(ValueTypeID::Integer)(v);}
                   );

  } else if (type == "std::vector<unsigned int, std::allocator<unsigned int> >"            ) {
    auto src = std::any_cast< std::vector<unsigned int> >(value);
    newVal.reserve(src.size());
    std::transform(src.begin(), src.end(),
                   std::back_inserter(newVal),
                   [] (auto & v) {return PARROT_TYPE(ValueTypeID::Integer)(v);}
                   );

  } else if (type == "std::vector<unsigned long, std::allocator<unsigned long> >"          ) {
    auto src = std::any_cast< std::vector<unsigned long> >(value);
    newVal.reserve(src.size());
    std::transform(src.begin(), src.end(),
                   std::back_inserter(newVal),
                   [] (auto & v) {return PARROT_TYPE(ValueTypeID::Integer)(v);}
                   );

  } else if (type == "std::vector<unsigned long long, std::allocator<unsigned long long> >") {
    auto src = std::any_cast< std::vector<unsigned long long> >(value);
    newVal.reserve(src.size());
    std::transform(src.begin(), src.end(),
                   std::back_inserter(newVal),
                   [] (auto & v) {return PARROT_TYPE(ValueTypeID::Integer)(v);}
                   );

  }

  // no else clause -- default CTor automatically makes empty list.

  value = newVal;
}
// .......................................................................... //
void Descriptor::rectify_RealList   () {
  PARROT_TYPE(ValueTypeID::RealList) newVal;

  auto type = BCG::demangle( value.type().name() );

  if        (type == "std::vector<float, std::allocator<float> >"            ) {
    auto src = std::any_cast< std::vector<char> >(value);
    newVal.reserve(src.size());
    std::transform(src.begin(), src.end(),
                   std::back_inserter(newVal),
                   [] (auto & v) {return PARROT_TYPE(ValueTypeID::Real)(v);}
                   );

  } else if (type == "std::vector<double, std::allocator<double> >"          ) {
    auto src = std::any_cast< std::vector<double> >(value);
    newVal.reserve(src.size());
    std::transform(src.begin(), src.end(),
                   std::back_inserter(newVal),
                   [] (auto & v) {return PARROT_TYPE(ValueTypeID::Real)(v);}
                   );

  } else if (type == "std::vector<long double, std::allocator<long double> >") {
    auto src = std::any_cast< std::vector<long double> >(value);
    newVal.reserve(src.size());
    std::transform(src.begin(), src.end(),
                   std::back_inserter(newVal),
                   [] (auto & v) {return PARROT_TYPE(ValueTypeID::Real)(v);}
                   );

  }

  // no else clause -- default CTor automatically makes empty list.

  value = newVal;
}
// .......................................................................... //
void Descriptor::rectify_BooleanList() {value = std::any_cast< std::vector<bool> >(value);}

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
{
    BCG::trim(key);
}

// ========================================================================== //
// Getters

const std::string Descriptor::getKey            () const {return key;}
// .......................................................................... //
std::any          Descriptor::getValue          () const {return value;}
// .......................................................................... //
ValueTypeID       Descriptor::getValueTypeID    () const {return valueTypeID;}
// .......................................................................... //
const std::string Descriptor::getValueTypeName  () const {return valueTypeName(valueTypeID);}
// .......................................................................... //
const std::string Descriptor::getTypeID         () const {return value.type().name();}
// .......................................................................... //
const std::string Descriptor::getTypeIDDemangled() const {return BCG::demangle(value.type().name());}
// -------------------------------------------------------------------------- //
bool              Descriptor::isKeyCaseSensitive       () const {return keyCaseSensitive;}
bool              Descriptor::isValueCaseSensitive     () const {return valueCaseSensitive;}
// -------------------------------------------------------------------------- //
bool              Descriptor::isTrimLeadingWhitespaces () const {return trimLeadingWhitespaces;}
bool              Descriptor::isTrimTrailingWhitespaces() const {return trimTrailingWhitespaces;}
// -------------------------------------------------------------------------- //
bool              Descriptor::isMandatory              () const {return mandatory;}
// -------------------------------------------------------------------------- //
char              Descriptor::getListSeparator         () const {return listSeparator;}
// -------------------------------------------------------------------------- //
const std::vector<Restriction>                                                                   & Descriptor::getRestrictions () const {return restrictions;}
// -------------------------------------------------------------------------- //
const std::vector<std::pair<PARROT_TYPE(ValueTypeID::String), PARROT_TYPE(ValueTypeID::String)>> & Descriptor::getSubstitutions() const {return substitutions;}
const std::function<PARROT_TYPE(ValueTypeID::String) (const PARROT_TYPE(ValueTypeID::String) &)> & Descriptor::getUserPreParser() const {return userPreParser;}

// ========================================================================== //
// Setters

void Descriptor::reset () {
  resetKey     ();
  resetValue   ();
  resetMetaData();
  resetParsing ();
}
// .......................................................................... //
void Descriptor::resetKey     () {key = "";}
// .......................................................................... //
void Descriptor::resetValue   () {
  value.reset();
  valueTypeID = ValueTypeID::None;
}
// .......................................................................... //
void Descriptor::resetMetaData() {
  keyCaseSensitive        = false;
  valueCaseSensitive      = false;

  trimLeadingWhitespaces  = true;
  trimTrailingWhitespaces = true;

  mandatory               = false;

  listSeparator           = ',';
}
// .......................................................................... //
void Descriptor::resetParsing() {
  restrictions .clear();
  substitutions.clear();
  userPreParser = nullptr;
}
// -------------------------------------------------------------------------- //
void Descriptor::setKey (const std::string & newVal) {
    key = newVal;
    BCG::trim(key);
}
// -------------------------------------------------------------------------- //
void Descriptor::setValueAny (std::any    newVal, bool resetMetaData) {
  valueTypeID = getAnyValueType(newVal);
  value = newVal;
  rectify();

  resetParsing();
  if (resetMetaData) {this->resetMetaData();}
}
// -------------------------------------------------------------------------- //
void Descriptor::setValueType(ValueTypeID newVal, bool resetMetaData) {
  valueTypeID = newVal;
  value.reset();

  resetParsing();
  if (resetMetaData) {this->resetMetaData();}
}
// -------------------------------------------------------------------------- //
void Descriptor::setKeyCaseSensitive        (bool newVal) {  keyCaseSensitive = newVal;}
void Descriptor::setValueCaseSensitive      (bool newVal) {valueCaseSensitive = newVal;}
// .......................................................................... //
void Descriptor::setTrimLeadingWhitespaces  (bool newVal) {trimLeadingWhitespaces  = newVal;}
void Descriptor::setTrimTrailingWhitespaces (bool newVal) {trimTrailingWhitespaces = newVal;}
// .......................................................................... //
void Descriptor::setMandatory               (bool newVal) {mandatory = newVal;}
// -------------------------------------------------------------------------- //
void Descriptor::setListSeparator     (const char newVal) {listSeparator = newVal;}
// -------------------------------------------------------------------------- //
void Descriptor::addRestriction (const Restriction & restriction) {
  // check whether restriction is applicable to current value type

  if (valueTypeID == ValueTypeID::None) {throw std::runtime_error(THROWTEXT("    cannot add restrictions to empty type."));}

  switch ( restriction.getRestrictionValueTypeID() ) {
    case RestrictionValueTypeID::None        :
      break;

    case RestrictionValueTypeID::String      :
      if (
        valueTypeID != ValueTypeID::String     &&
        valueTypeID != ValueTypeID::StringList
      ) {throw std::runtime_error(THROWTEXT("    Restriction incompatible with value type"));}
      break;

    case RestrictionValueTypeID::Integer     :
      if (
        valueTypeID != ValueTypeID::Integer     &&
        valueTypeID != ValueTypeID::IntegerList
      ) {throw std::runtime_error(THROWTEXT("    Restriction incompatible with value type"));}
      break;

    case RestrictionValueTypeID::Real        :
      if (
        valueTypeID != ValueTypeID::Real     &&
        valueTypeID != ValueTypeID::RealList
      ) {throw std::runtime_error(THROWTEXT("    Restriction incompatible with value type"));}
      break;

    case RestrictionValueTypeID::Numeric     :
      if (
        valueTypeID != ValueTypeID::Integer     &&
        valueTypeID != ValueTypeID::IntegerList &&
        valueTypeID != ValueTypeID::Real        &&
        valueTypeID != ValueTypeID::RealList
      ) {throw std::runtime_error(THROWTEXT("    Restriction incompatible with value type"));}
      break;

    case RestrictionValueTypeID::Boolean     :
      if (
        valueTypeID != ValueTypeID::Boolean     &&
        valueTypeID != ValueTypeID::BooleanList
      ) {throw std::runtime_error(THROWTEXT("    Restriction incompatible with value type"));}
      break;

    case RestrictionValueTypeID::StringList  :
      if (valueTypeID != ValueTypeID::StringList) {throw std::runtime_error(THROWTEXT("    Restriction incompatible with value type"));}
      break;

    case RestrictionValueTypeID::IntegerList :
      if (valueTypeID != ValueTypeID::IntegerList) {throw std::runtime_error(THROWTEXT("    Restriction incompatible with value type"));}
      break;

    case RestrictionValueTypeID::RealList    :
      if (valueTypeID != ValueTypeID::RealList) {throw std::runtime_error(THROWTEXT("    Restriction incompatible with value type"));}
      break;

    case RestrictionValueTypeID::BooleanList :
      if (valueTypeID != ValueTypeID::BooleanList) {throw std::runtime_error(THROWTEXT("    Restriction incompatible with value type"));}
      break;

  }

  // actually do the trick
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
void Descriptor::setUserPreParser(const std::function<std::string (const std::string &)> & uFunc) {
  if ( !uFunc ) {throw std::runtime_error(THROWTEXT("    Uninitialized parsing function"));}
  userPreParser = uFunc;
}
void Descriptor::clearUserPreParser() {userPreParser = nullptr;}
// -------------------------------------------------------------------------- //
void Descriptor::makeRanged(const std::string &         K,
                            ValueTypeID                 T,
                            PARROT_TYPE(ValueTypeID::Real) min,
                            PARROT_TYPE(ValueTypeID::Real) max,
                            RestrictionViolationPolicy  policy,
                            const std::string &         restrictionViolationText,
                            bool                        M
) {
  reset();
  setKey(K);
  valueTypeID = T;
  addRestriction( Restriction(min, max, policy, restrictionViolationText) );
  setMandatory(M);
}
// .......................................................................... //
void Descriptor::makeListboundPreParse(const std::string &                           K,
                                       ValueTypeID                                   T,
                                       const PARROT_TYPE(ValueTypeID::StringList) &  list,
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
void Descriptor::makeUserboundPreParse(const std::string &                                K,
                                       ValueTypeID                                        T,
                                       const std::function<bool (const PARROT_TYPE(ValueTypeID::String) &)> &  uFunc,
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
  else                {reVal << " for keyowrd '" << key << "'\n";}

  reVal << "  Datatype                 : " << valueTypeName(valueTypeID) << "\n";
  reVal << "  Default value            : " << (value.has_value() ? getAnyText(value) : "(### none ###)")  << "\n";

  reVal << std::boolalpha;
  reVal << "  Keyword case sensitive   : " << keyCaseSensitive        << "\n";
  reVal << "  Value case sensitive     : " << valueCaseSensitive      << "\n";
  reVal << "  Trim leading whitespaces : " << trimLeadingWhitespaces  << "\n";
  reVal << "  Trim trailing whitespaces: " << trimTrailingWhitespaces << "\n";
  reVal << "  Keyword mandatory        : " << mandatory               << "\n";

  if ( userPreParser ) {
    auto ptr = userPreParser.target<std::string (*) (const std::string &)>();
    reVal << "  with userdefined preparsing function at " << (void *) *ptr << "\n";
  }

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
