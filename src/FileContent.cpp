// ========================================================================= //
// dependencies

// STL
#include <stdexcept>

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

FileContent::FileContent (const std::string & key,
                          const std::any &    value,
                          bool                foundInFile,
                          bool                triggeredWarning
) {
  content[key] = std::make_tuple(value, getAnyValueType(value), foundInFile, triggeredWarning);
}

// ========================================================================== //
// Getters

std::string              FileContent::getSource  () const {return source;}
// -------------------------------------------------------------------------- //
bool                     FileContent::hasKeyword          (const std::string & key) const {return content.contains(key);}
bool                     FileContent::hasValue            (const std::string & key) const {return getValue(key).has_value();}
// .......................................................................... //
FileContent::ContentType FileContent::get                 (const std::string & key) const {return                                (*content.find(key)).second ;}
std::any                 FileContent::getValue            (const std::string & key) const {return std::get<FCE_Value           >((*content.find(key)).second);}
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
// Setters

void FileContent::addElement (const std::string & key,
                              const std::any &    value,
                              bool                foundInFile,
                              bool                triggeredWarning
) {
  if ( hasKeyword(key) ) {throw std::runtime_error(THROWTEXT("    key already defined!"));}
  content[key] = std::make_tuple(value, getAnyValueType(value), foundInFile, triggeredWarning);;
}

// ========================================================================== //
// Random Access
