// ========================================================================= //
// dependencies

// STL
#include <stdexcept>

#include <iostream>

#include <string>
using namespace std::string_literals;

#include <algorithm>

// own
#include "BCG.hpp"
#include "Parrot/FileContent.hpp"

using namespace Parrot;

// ========================================================================== //
// local macro

#define THROWTEXT(msg) ("RUNTIME EXCEPTION IN "s + (__PRETTY_FUNCTION__) + "\n"s + msg)

// ========================================================================== //
// CTors

FileContent::FileContent (const std::string & source) {this->source = source;}

// ========================================================================== //
// Getters

std::string              FileContent::getSource  () const {return source;}
// -------------------------------------------------------------------------- //
bool                     FileContent::hasKeyword          (const std::string & key) const {return content.contains(key);}
bool                     FileContent::hasValue            (const std::string & key) const {return ((*this)[key]).has_value();}
// .......................................................................... //
FileContent::ContentType FileContent::get                 (const std::string & key) const {return                                (*content.find(key)).second ;}
Parrot::ValueTypeID      FileContent::getValueType        (const std::string & key) const {return std::get<FCE_ValueType       >((*content.find(key)).second);}
bool                     FileContent::getFoundInFile      (const std::string & key) const {return std::get<FCE_FoundInFile     >((*content.find(key)).second);}
bool                     FileContent::getTriggeredWarning (const std::string & key) const {return std::get<FCE_TriggeredWarning>((*content.find(key)).second);}
// -------------------------------------------------------------------------- //
std::vector<std::string> FileContent::getKeywords() const {
  std::vector<std::string> reVal(content.size());

  std::transform(content.begin(), content.end(),
                 reVal  .begin(),
                 [] (const auto & pair) {return pair.first;}
  );

  return reVal;
}

// ========================================================================== //
// Value Access

std::any                 FileContent::operator[] (const std::string & key) const {return std::get<FCE_Value>((*content.find(key)).second);}
// .......................................................................... //
FileContent::TypeConverterClass FileContent::getValueX (const std::string & key) const {return (*this)[key];}

// ========================================================================== //
// Setters

void FileContent::reset() {

}
// -------------------------------------------------------------------------- //
void FileContent::addElement (const std::string & key,
                              const std::any &    value,
                              bool                foundInFile,
                              bool                triggeredWarning
) {
  if ( hasKeyword(key) ) {throw std::runtime_error(THROWTEXT("    keyword already defined!"));}
  content[key] = std::make_tuple(value, getAnyValueType(value), foundInFile, triggeredWarning);;
}

// ========================================================================== //
// Type Converter Class

FileContent::TypeConverterClass::operator PARROT_TYPE(ValueTypeID::Integer) () {return std::any_cast<PARROT_TYPE(ValueTypeID::Integer)>(data);}
