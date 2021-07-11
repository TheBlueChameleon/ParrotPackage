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
// local constants

enum ContentElements {
  CE_Value,
  CE_ValueType,
  CE_Found,
  CE_TriggeredError
};

// ========================================================================== //
// CTors

FileContent::FileContent (const std::string & key, const std::any x) {
  FileContent::ContentType entry = std::make_tuple(x, getAnyValueType(x), false, false);

  content[key] = entry;
}

// ========================================================================== //
// Getters

std::string              FileContent::getSource  () const {return source;}
// .......................................................................... //
bool                     FileContent::hasKeyword      (const std::string & key) const {return content.contains(key);}
FileContent::ContentType FileContent::get        (const std::string & key) const {return                             (*content.find(key)).second ;}
std::any                 FileContent::getValue        (const std::string & key) const {return std::get<CE_Value         >((*content.find(key)).second);}
Parrot::ValueTypeID      FileContent::getValueType    (const std::string & key) const {return std::get<CE_ValueType     >((*content.find(key)).second);}
bool                     FileContent::wasFoundInFile  (const std::string & key) const {return std::get<CE_Found         >((*content.find(key)).second);}
bool                     FileContent::triggeredWarning(const std::string & key) const {return std::get<CE_TriggeredError>((*content.find(key)).second);}
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

// ========================================================================== //
// Random Access
