// ========================================================================= //
// dependencies

// STL
#include <stdexcept>

#include <string>
using namespace std::string_literals;

#include <tuple>

#include <algorithm>

// own
#include "BCG.hpp"
#include "Parrot/Reader.hpp"
#include "Parrot/FileContent.hpp"

using namespace Parrot;

// ========================================================================== //
// local macro

#define THROWTEXT(msg) ("RUNTIME EXCEPTION IN "s + (__PRETTY_FUNCTION__) + "\n"s + msg)

// ========================================================================== //
// Parsing machinery

void Reader::descriptorValidityCheck(const Parrot::Descriptor & descriptor) const {
  auto keyword = descriptor.getKey();
  if ( keyword.empty()     ) {throw InvalidDescriptorError(THROWTEXT("    no keyword name specified!"));}
  if ( hasKeyword(keyword) ) {throw InvalidDescriptorError(THROWTEXT("    keyword '" + descriptor.getKey() + "' already registered!"));}
}
// -------------------------------------------------------------------------- //
void Reader::parseLine() {
  if (linePreparsed.empty())             {return;}
  if (linePreparsed[0] == commentMarker) {return;}

  auto separationIdx = linePreparsed.find('=');

  std::cout << "lineOriginal:" << std::endl;
  std::cout <<  lineOriginal   << std::endl;
  std::cout << "linePreparsed:" << std::endl;
  std::cout <<  linePreparsed   << std::endl;
  std::cout << std::endl;

  if (separationIdx == std::string::npos) {
    if (verbose) {
      BCG::writeWarning("found no value in line #" + std::to_string(linenumber) + ":\n" +
                        lineOriginal
      );
    }
    return;
  }

  keyword = linePreparsed.substr(0, separationIdx);
  BCG::trim(keyword);
}

// ========================================================================== //
// CTors

Reader::Reader(const std::vector<Descriptor> & descriptors) :
  descriptors(descriptors)
{}

// ========================================================================== //
// Getters

char                                    Reader::getCommentMarker        () const {return commentMarker        ;}
char                                    Reader::getMultilineMarker      () const {return multilineMarker      ;}
char                                    Reader::getAssignmentMarker     () const {return assignmentMarker     ;}
bool                                    Reader::getKeywordCaseSensitive () const {return keywordCaseSensitive ;}
bool                                    Reader::getVerbose              () const {return verbose              ;}
// -------------------------------------------------------------------------- //
const MissingKeywordPolicy &            Reader::getMissingKeywordPolicyMandatory  () const {return missingKeywordPolicyMandatory  ;}
const std::string          &            Reader::getMissingKeywordTextMandatory    () const {return missingKeywordTextMandatory    ;}
const MissingKeywordPolicy &            Reader::getMissingKeywordPoliyNonMandatory() const {return missingKeywordPoliyNonMandatory;}
const std::string          &            Reader::getMissingKeywordTextNonMandatory () const {return missingKeywordTextNonMandatory ;}
const MissingKeywordPolicy &            Reader::getUnexpectedKeywordPolicy        () const {return unexpectedKeywordPolicy        ;}
const std::string          &            Reader::getUnexpectedKeywordText          () const {return unexpectedKeywordText          ;}
// -------------------------------------------------------------------------- //
size_t                                  Reader::size      () const {return descriptors.size();}
bool                                    Reader::hasKeyword(const std::string & keyword) const {
  auto end = descriptors.end();
  return std::find_if(descriptors.begin(), end, [keyword] (const auto & descriptor) {return descriptor.getKey() == keyword;}) != end;
}
// .......................................................................... //
const std::vector<Descriptor> & Reader::getDescriptors()                            const {return descriptors;}
const             Descriptor  & Reader::getDescriptor (const size_t        idx    ) const {
  if (idx >= descriptors.size()) {throw std::out_of_range(THROWTEXT("    index out of bounds!"));}
  return descriptors[idx];
}
const             Descriptor  & Reader::getDescriptor (const std::string & keyword) const {
  auto end  = descriptors.end();
  auto spot = std::find_if(descriptors.begin(), end, [keyword] (const auto & descriptor) {return descriptor.getKey() == keyword;});

  if (spot == end) {throw std::out_of_range(THROWTEXT("    keyword does not exist!"));}

  return *spot;
}

// ========================================================================== //
// Setters

void Reader::reset() {
  commentMarker                   = '#';
  multilineMarker                 = '\\';
  keywordCaseSensitive            = false;
  verbose                         = true;

  missingKeywordPolicyMandatory   = MissingKeywordPolicy::Warning;
  missingKeywordTextMandatory     = "mandatory keyword $K was not found in file $F!";
  missingKeywordPoliyNonMandatory = MissingKeywordPolicy::Exception;
  missingKeywordTextNonMandatory  = "keyword $K was not found; reverting to default ($D)";
  unexpectedKeywordPolicy         = MissingKeywordPolicy::Warning;
  unexpectedKeywordText           = "unexpected keyword in file $F! (Taken as string keyword)\n$L";

  resetKeywords();
}
// .......................................................................... //
void Reader::resetKeywords() {descriptors.clear();}
// -------------------------------------------------------------------------- //
void Reader::setMissingKeywordPolicyMandatory  (const MissingKeywordPolicy & newVal) {missingKeywordPolicyMandatory   = newVal;}
void Reader::setMissingKeywordTextMandatory    (const std::string          & newVal) {missingKeywordTextMandatory     = newVal;}
void Reader::setMissingKeywordPoliyNonMandatory(const MissingKeywordPolicy & newVal) {missingKeywordPoliyNonMandatory = newVal;}
void Reader::setMissingKeywordTextNonMandatory (const std::string          & newVal) {missingKeywordTextNonMandatory  = newVal;}
void Reader::setUnexpectedKeywordPolicy        (const MissingKeywordPolicy & newVal) {unexpectedKeywordPolicy         = newVal;}
void Reader::setUnexpectedKeywordText          (const std::string          & newVal) {unexpectedKeywordText           = newVal;}
// -------------------------------------------------------------------------- //
void Reader::setCommentMarker                  (char                         newVal) {commentMarker         = newVal;}
void Reader::setMultilineMarker                (char                         newVal) {multilineMarker       = newVal;}
void Reader::setAssignmentMarker               (char                         newVal) {assignmentMarker      = newVal;}
void Reader::setKeywordCaseSensitive           (bool                         newVal) {keywordCaseSensitive  = newVal;}
void Reader::setVerbose                        (bool                         newVal) {verbose               = newVal;}
// -------------------------------------------------------------------------- //
void Reader::addKeyword                  (const             Parrot::Descriptor  & descriptor ) {
  descriptorValidityCheck(descriptor);
  descriptors.push_back(descriptor);
}
// .......................................................................... //
void Reader::addKeyword                  (const Parrot::Reader::MinimalDescriptor & descriptor) {
  auto descriptor_full = Parrot::Descriptor(
    std::get<0>(descriptor),
    std::get<1>(descriptor),
    std::get<2>(descriptor)
  );
  descriptorValidityCheck(descriptor_full);
  descriptors.push_back(descriptor_full);
}
// .......................................................................... //
void Reader::addKeyword                  (const std::string &                           keyword,
                                          ValueTypeID                                   valueType,
                                          bool                                          mandatory
) {
  auto descriptor = Descriptor(keyword, valueType, mandatory);
  descriptorValidityCheck(descriptor);
  addKeyword(descriptor);
}
// .......................................................................... //
void Reader::addKeyword                  (const std::string &                           keyword,
                                          std::any &                                    defaultValue,
                                          bool                                          mandatory
) {
  auto descriptor = Descriptor();
  descriptor.setKey       (keyword);
  descriptor.setValueAny  (defaultValue);
  descriptor.setMandatory (mandatory);
  descriptorValidityCheck (descriptor);
  addKeyword(descriptor);
}
// -------------------------------------------------------------------------- //
void Reader::addKeywords                 (const std::vector<Parrot::Descriptor> & descriptors) {
  for (auto & descriptor : descriptors) {descriptorValidityCheck(descriptor);}
  BCG::append_to_vector(this->descriptors, descriptors);
}
// .......................................................................... //
void Reader::addKeywords                 (const std::vector<Parrot::Reader::MinimalDescriptor> & descriptors) {
  for (auto & descriptor : descriptors) {addKeyword(descriptor);}
}
// .......................................................................... //
void Reader::addKeywords                 (const std::vector<std::string> &              keywords,
                                          const std::vector<Parrot::ValueTypeID> &      valueTypes,
                                          const std::vector<bool> &                     mandatory
) {
  bool useMandatory = !mandatory.empty();
  auto N = keywords.size();

  if (                 valueTypes.size() != N ||
      (useMandatory && mandatory .size() != N)
  ) {throw std::logic_error("    Count of description elements does not match count of keywords!");}

  for (auto i = 0u; i < N; ++i) {addKeyword(keywords[i], valueTypes[i], useMandatory ? mandatory[i] : true);}
}
// .......................................................................... //
void Reader::addKeywords                 (const std::vector<std::string> &              keywords,
                                          const std::vector<std::any> &                 defaultValues,
                                          const std::vector<bool> &                     mandatory
) {

  bool useMandatory = !mandatory.empty();
  auto N = keywords.size();

  if (                 defaultValues.size() != N ||
      (useMandatory && mandatory    .size() != N)
  ) {throw std::logic_error("    Count of description elements does not match count of keywords!");}

  for (auto i = 0u; i < N; ++i) {addKeyword(keywords[i], defaultValues[i], useMandatory ? mandatory[i] : true);}
}
// -------------------------------------------------------------------------- //
void Reader::addKeywordRanged            (const std::string &                           keyword,
                                          ValueTypeID                                   valueType,
                                          PARROT_TYPE(ValueTypeID::Real)                min,
                                          PARROT_TYPE(ValueTypeID::Real)                max,
                                          RestrictionViolationPolicy                    policy,
                                          const std::string &                           restrictionViolationText,
                                          bool                                          mandatory
) {
  auto descriptor = Descriptor();
  descriptor.makeRanged(keyword, valueType, min, max, policy, restrictionViolationText, mandatory);
  descriptorValidityCheck (descriptor);
  addKeyword              (descriptor);
}
// .......................................................................... //
void Reader::addKeywordListboundPreParse (const std::string &                           keyword,
                                          ValueTypeID                                   valueType,
                                          const PARROT_TYPE(ValueTypeID::StringList) &  list,
                                          bool                                          forbiddenList,
                                          RestrictionViolationPolicy                    policy,
                                          const std::string &                           restrictionViolationText,
                                          bool                                          mandatory
) {
  auto descriptor = Descriptor();
  descriptor.makeListboundPreParse(keyword, valueType, list, forbiddenList, policy, restrictionViolationText, mandatory);
  descriptorValidityCheck (descriptor);
  addKeyword              (descriptor);
}
// .......................................................................... //
void Reader::addKeywordUserboundPreParse (const std::string &                           keyword,
                                          ValueTypeID                                   valueType,
                                          const std::function< bool(const PARROT_TYPE(ValueTypeID::String) &)> &uFunc,
                                          RestrictionViolationPolicy                    policy,
                                          const std::string &                           restrictionViolationText,
                                          bool                                          mandatory
) {
  auto descriptor = Descriptor();
  descriptor.makeUserboundPreParse(keyword, valueType, uFunc, policy, restrictionViolationText, mandatory);
  descriptorValidityCheck (descriptor);
  addKeyword              (descriptor);
}

// ========================================================================== //
// I/O

Parrot::FileContent Reader::operator() (const std::string & source) {
  std::fstream hFile = BCG::openThrow(source, std::fstream::in);
  content            = FileContent(source);
  lineOriginal  = "";
  linePreparsed = "";
  keyword       = "";
  defaultValue  = "";
  readValue     = "";
  linenumber    = -1;
  foundInFile   = std::vector<bool> ();

  std::string linebuffer;
  for (;std::getline(hFile, linebuffer);) {
    lineOriginal += linebuffer;
    ++linenumber;

    BCG::trim(linebuffer);

    if (linebuffer.back() == multilineMarker) {
      linebuffer.pop_back();
      linePreparsed += linebuffer;
      lineOriginal  += "\n";
      continue;
    }

    linePreparsed += linebuffer;
    parseLine();

    lineOriginal  = "";
    linePreparsed = "";
    keyword       = "";
    defaultValue  = "";
    readValue     = "";
  }

  return content;
}
// -------------------------------------------------------------------------- //
std::string Reader::to_string() const {
  std::string reVal = "Parrot::Reader object\n";
  reVal += "  comment marker                  : "s + (commentMarker         ? std::string(1, commentMarker  ) : "(none)"s) + "\n";
  reVal += "  line continuation marker        : "s + (multilineMarker       ? std::string(1, multilineMarker) : "(none)"s) + "\n";
  reVal += "  assignment marker               : "s + (assignmentMarker                                                   ) + "\n";
  reVal += "  treat keywords case sensitively : "s + (keywordCaseSensitive  ?                           "yes" : "no"     ) + "\n";
  reVal += "  verbose mode                    : "s + (verbose               ?                           "yes" : "no"     ) + "\n";

  reVal += "ready to extract these objects:\n";

  for (const auto & descriptor : descriptors) {reVal += descriptor.to_string();}

  return reVal;
}
