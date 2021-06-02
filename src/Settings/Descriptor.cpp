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
// CTor, DTor

// ========================================================================== //
// Getters

std::string   Descriptor::getKey      () const {return key;}
std::any      Descriptor::getValue    () const {return value;}
ValueType     Descriptor::getValueType() const {return valueType;}
// -------------------------------------------------------------------------- //
bool          Descriptor::isKeyCaseSensitive       () const {return keyCaseSensitive;}
bool          Descriptor::isValueCaseSensitive     () const {return valueCaseSensitive;}
// -------------------------------------------------------------------------- //
bool          Descriptor::isTrimLeadingWhitespaces () const {return trimLeadingWhitespaces;}
bool          Descriptor::isTrimTrailingWhitespaces() const {return trimTrailingWhitespaces;}
// -------------------------------------------------------------------------- //
bool          Descriptor::isMandatory              () const {return mandatory;}
// -------------------------------------------------------------------------- //
const std::vector<Restriction>                          & Descriptor::getRestrictions() const {return restrictions;}
// -------------------------------------------------------------------------- //
const std::vector<std::pair<std::string, std::string>>  & Descriptor::getSubstitutions() const {return substitutions;}
const std::function<std::string (const std::string &)>  & Descriptor::getUserParser   () const {return userParser;}

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
  userParser = nullptr;
}
// -------------------------------------------------------------------------- //
void Descriptor::setKey (const std::string & newVal) {key = newVal;}
// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //
void Descriptor::setKeyCaseSensitive        (bool newVal) {  keyCaseSensitive = newVal;}
void Descriptor::setValueCaseSensitive      (bool newVal) {valueCaseSensitive = newVal;}
// .......................................................................... //
void Descriptor::setTrimLeadingWhitespaces  (bool newVal) {trimLeadingWhitespaces  = newVal;}
void Descriptor::setTrimTrailingWhitespaces (bool newVal) {trimTrailingWhitespaces = newVal;}
// .......................................................................... //
void Descriptor::setMandatory               (bool newVal) {mandatory = newVal;}
