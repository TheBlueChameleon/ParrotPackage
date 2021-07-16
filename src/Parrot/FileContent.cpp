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
// safe getter

FileContent::ContentType              FileContent::getSafe             (const std::string & key) const {
  auto it = content.find(key);

  if ( it == content.end() ) {
    throw Parrot::ValueAccessError(THROWTEXT("    keyword '" + key + "' does not exist."));
  }

  return (*it).second;
}

// ========================================================================== //
// CTors

FileContent::FileContent (const std::string & source) {this->source = source;}

// ========================================================================== //
// Getters

const std::string &                   FileContent::getSource           ()                        const {return source;}
bool                                  FileContent::empty               ()                        const {return content.empty();}
size_t                                FileContent::size                ()                        const {return content.size();}
// -------------------------------------------------------------------------- //
bool                                  FileContent::hasKeyword          (const std::string & key) const {return content.contains(key);}
bool                                  FileContent::hasValue            (const std::string & key) const {return getAny(key).has_value();}
std::vector<std::string>              FileContent::getKeywords() const {
  std::vector<std::string> reVal(content.size());

  std::transform(content.begin(), content.end(),
                 reVal  .begin(),
                 [] (const auto & pair) {return pair.first;}
  );

  return reVal;
}
// .......................................................................... //
FileContent::ContentType              FileContent::get                 (const std::string & key) const {return                            getSafe(key) ;}
std::any                              FileContent::getAny              (const std::string & key) const {return std::get<Value           >(getSafe(key));}
Parrot::ValueTypeID                   FileContent::getValueType        (const std::string & key) const {return std::get<ValueType       >(getSafe(key));}
bool                                  FileContent::getFoundInFile      (const std::string & key) const {return std::get<FoundInFile     >(getSafe(key));}
bool                                  FileContent::getTriggeredWarning (const std::string & key) const {return std::get<TriggeredWarning>(getSafe(key));}
// .......................................................................... //
PARROT_TYPE(ValueTypeID::String     ) FileContent::get_String          (const std::string & key) const {
  return static_cast<PARROT_TYPE(ValueTypeID::String     )>((*this)[key]);
}
PARROT_TYPE(ValueTypeID::Integer    ) FileContent::get_Integer         (const std::string & key) const {
  return static_cast<PARROT_TYPE(ValueTypeID::Integer    )>((*this)[key]);
}
PARROT_TYPE(ValueTypeID::Real       ) FileContent::get_Real            (const std::string & key) const {
  return static_cast<PARROT_TYPE(ValueTypeID::Real       )>((*this)[key]);
}
PARROT_TYPE(ValueTypeID::Boolean    ) FileContent::get_Boolean         (const std::string & key) const {
  return static_cast<PARROT_TYPE(ValueTypeID::Boolean    )>((*this)[key]);
}
PARROT_TYPE(ValueTypeID::StringList ) FileContent::get_StringList      (const std::string & key) const {
  return static_cast<PARROT_TYPE(ValueTypeID::StringList )>((*this)[key]);
}
PARROT_TYPE(ValueTypeID::IntegerList) FileContent::get_IntegerList     (const std::string & key) const {
  return static_cast<PARROT_TYPE(ValueTypeID::IntegerList)>((*this)[key]);
}
PARROT_TYPE(ValueTypeID::RealList   ) FileContent::get_RealList        (const std::string & key) const {
  return static_cast<PARROT_TYPE(ValueTypeID::RealList   )>((*this)[key]);
}
PARROT_TYPE(ValueTypeID::BooleanList) FileContent::get_BooleanList     (const std::string & key) const {
  return static_cast<PARROT_TYPE(ValueTypeID::BooleanList)>((*this)[key]);
}
// -------------------------------------------------------------------------- //

// -------------------------------------------------------------------------- //
const std::map<std::string,           FileContent::ContentType> & FileContent::getContent() const {return content;}

// ========================================================================== //
// Value Access

FileContent::TypeConverterClass FileContent::operator[](const std::string & key) const {return getAny(key);}

// ========================================================================== //
// Setters

void FileContent::reset() {
  source = "<user defined>";
  content.clear();
}
// -------------------------------------------------------------------------- //
void FileContent::addElement   (const std::string & key,
                                const std::any &    value,
                                bool                foundInFile,
                                bool                triggeredWarning
) {
  if ( hasKeyword(key) )    {throw Parrot::ValueAccessError(THROWTEXT("    keyword already defined!"));}
  if ( !value.has_value() ) {throw Parrot::ValueAccessError(THROWTEXT("    value type could not be deduced!"));}
  content[key] = std::make_tuple(value, getAnyValueType(value), foundInFile, triggeredWarning);
}
// .......................................................................... //
void FileContent::addElement   (const std::string & key,
                                const ValueTypeID   valueType,
                                bool                foundInFile,
                                bool                triggeredWarning
) {
  if ( hasKeyword(key) )    {throw Parrot::ValueAccessError(THROWTEXT("    keyword already defined!"));}
  content[key] = std::make_tuple(std::any(), valueType, foundInFile, triggeredWarning);
}
// -------------------------------------------------------------------------- //
void FileContent::updateElement(const std::string & key,
                                const std::any &    value,
                                bool                foundInFile,
                                bool                triggeredWarning
) {
  if (!hasKeyword(key) ) {throw Parrot::ValueAccessError(THROWTEXT("    keyword does not exist!"));}
  if ( !value.has_value() ) {throw Parrot::ValueAccessError(THROWTEXT("    value type could not be deduced!"));}
  content[key] = std::make_tuple(value, getAnyValueType(value), foundInFile, triggeredWarning);
}
// .......................................................................... //
void FileContent::updateElement(const std::string & key,
                                const ValueTypeID   valueType,
                                bool                foundInFile,
                                bool                triggeredWarning
) {
  if (!hasKeyword(key) ) {throw Parrot::ValueAccessError(THROWTEXT("    keyword does not exist!"));}
  content[key] = std::make_tuple(std::any(), valueType, foundInFile, triggeredWarning);
}
// -------------------------------------------------------------------------- //
void FileContent::setElement   (const std::string & key,
                                const std::any &    value,
                                bool                foundInFile,
                                bool                triggeredWarning
) {
  if ( !value.has_value() ) {throw Parrot::ValueAccessError(THROWTEXT("    value type could not be deduced!"));}
  content[key] = std::make_tuple(value, getAnyValueType(value), foundInFile, triggeredWarning);
}
// .......................................................................... //
void FileContent::setElement   (const std::string & key,
                                const ValueTypeID   valueType,
                                bool                foundInFile,
                                bool                triggeredWarning
) {
  content[key] = std::make_tuple(std::any(), valueType, foundInFile, triggeredWarning);
}

// ========================================================================== //
// Representation

std::string FileContent::to_string() const {
  size_t                   N = content.size(), i = 0;
  std::vector<std::string>  keys(N),  types(N),  contents(N),  flagsFound(N),  flagsWarning(N);
  size_t                   wKeys   , wTypes   , wContents   , wFlagsFound   , wFlagsWarning   ;

  for (const auto & [key, data] : content) {
    keys        [i] = key;
    types       [i] = valueTypeName(std::get<ValueType       >(data));
    contents    [i] = getAnyText   (std::get<Value           >(data));
    flagsFound  [i] =               std::get<FoundInFile     >(data) ? "yes" : "no";
    flagsWarning[i] =               std::get<TriggeredWarning>(data) ? "yes" : "no";
    ++i;
  }

  wKeys = std::accumulate    (keys.begin(), keys.end(),
                              size_t(0),
                              [] (const auto & acc, const auto & elm) {return std::max(acc, elm.size());}
  );

  wTypes = std::accumulate   (types.begin(), types.end(),
                              size_t(0),
                              [] (const auto & acc, const auto & elm) {return std::max(acc, elm.size());}
  );

  wContents = std::accumulate(contents.begin(), contents.end(),
                              size_t(0),
                              [] (const auto & acc, const auto & elm) {return std::max(acc, elm.size());}
  );

  wKeys         = std::max(wKeys    , ("keyword"s            ).size());
  wTypes        = std::max(wTypes   , ("data type"s          ).size());
  wContents     = std::max(wContents, ("content"s            ).size());
  wFlagsFound   =                     ("in File"s            ).size() ;
  wFlagsWarning =                     ("triggered Warning"s  ).size() ;

  std::string reVal = BCG::center("keyword"  , wKeys    ) + " | " +
                      BCG::center("content"  , wContents) + " | " +
                      BCG::center("data type", wTypes   ) + " | " +
                      "in File | " +
                      "triggered Warning" +
                      "\n";

  reVal += std::string(wKeys         + 1, '-') + "+" +
           std::string(wContents     + 2, '-') + "+" +
           std::string(wTypes        + 2, '-') + "+" +
           std::string(wFlagsFound   + 2, '-') + "+" +
           std::string(wFlagsWarning + 1, '-')  +
           "\n";

  for (i = 0; i < keys.size(); ++i) {
    reVal += BCG::justifyLeft(keys        [i], wKeys        ) + " | ";
    reVal += BCG::justifyLeft(contents    [i], wContents    ) + " | ";
    reVal += BCG::justifyLeft(types       [i], wTypes       ) + " | ";
    reVal += BCG::center     (flagsFound  [i], wFlagsFound  ) + " | ";
    reVal += BCG::center     (flagsWarning[i], wFlagsWarning)        ;
    reVal += "\n";
  }

  return reVal;
}

// ========================================================================== //
// Type Converter Class

FileContent::TypeConverterClass::operator PARROT_TYPE(ValueTypeID::String     ) () {return std::any_cast<PARROT_TYPE(ValueTypeID::String     )>(data);}
FileContent::TypeConverterClass::operator PARROT_TYPE(ValueTypeID::Integer    ) () {return std::any_cast<PARROT_TYPE(ValueTypeID::Integer    )>(data);}
FileContent::TypeConverterClass::operator PARROT_TYPE(ValueTypeID::Real       ) () {return std::any_cast<PARROT_TYPE(ValueTypeID::Real       )>(data);}
FileContent::TypeConverterClass::operator PARROT_TYPE(ValueTypeID::Boolean    ) () {return std::any_cast<PARROT_TYPE(ValueTypeID::Boolean    )>(data);}
FileContent::TypeConverterClass::operator PARROT_TYPE(ValueTypeID::StringList ) () {return std::any_cast<PARROT_TYPE(ValueTypeID::StringList )>(data);}
FileContent::TypeConverterClass::operator PARROT_TYPE(ValueTypeID::IntegerList) () {return std::any_cast<PARROT_TYPE(ValueTypeID::IntegerList)>(data);}
FileContent::TypeConverterClass::operator PARROT_TYPE(ValueTypeID::RealList   ) () {return std::any_cast<PARROT_TYPE(ValueTypeID::RealList   )>(data);}
FileContent::TypeConverterClass::operator PARROT_TYPE(ValueTypeID::BooleanList) () {return std::any_cast<PARROT_TYPE(ValueTypeID::BooleanList)>(data);}
