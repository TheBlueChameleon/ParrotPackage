// ========================================================================= //
// dependencies

// STL
#include <stdexcept>

#include <string>
using namespace std::string_literals;

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
  if ( keyword.empty()     ) {throw std::runtime_error(THROWTEXT("    no keyword name specified!"));}
  if ( hasKeyword(keyword) ) {throw std::runtime_error(THROWTEXT("    keyword '" + descriptor.getKey() + "' already registered!"));}
}

// ========================================================================== //
// CTors

Reader::Reader(const std::vector<Descriptor> & descriptors) :
  descriptors(descriptors)
{}

// ========================================================================== //
// Getters

char                                    Reader::getCommentMarker  () const {return commentMarker  ;}
char                                    Reader::getMultilineMarker() const {return multilineMarker;}
bool                                    Reader::getVerbose        () const {return verbose        ;}
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
  if (idx >= descriptors.size()) {throw std::runtime_error(THROWTEXT("    index out of bounds!"));}
  return descriptors[idx];
}
const             Descriptor  & Reader::getDescriptor (const std::string & keyword) const {
  auto end  = descriptors.end();
  auto spot = std::find_if(descriptors.begin(), end, [keyword] (const auto & descriptor) {return descriptor.getKey() == keyword;});

  if (spot == end) {throw std::runtime_error(THROWTEXT("    keyword does not exist!"));}

  return *spot;
}

// ========================================================================== //
// Setters

void Reader::reset() {
  commentMarker                   = '#';
  multilineMarker                 = '\\';
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
void Reader::setCommentMarker                  (char                         newVal) {commentMarker   = newVal;}
void Reader::setMultilineMarker                (char                         newVal) {multilineMarker = newVal;}
void Reader::setVerbose                        (bool                         newVal) {verbose         = newVal;}
// -------------------------------------------------------------------------- //
void Reader::addKeyword                  (const             Parrot::Descriptor  & descriptor ) {
  descriptorValidityCheck(descriptor);
  descriptors.push_back(descriptor);
}
// .......................................................................... //
void Reader::addKeywords                 (const std::vector<Parrot::Descriptor> & descriptors) {
  for (auto & descriptor : descriptors) {descriptorValidityCheck(descriptor);}
  BCG::append_to_vector(this->descriptors, descriptors);
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
  ) {throw std::runtime_error("    Count of description elements does not match count of keywords!");}

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
  ) {throw std::runtime_error("    Count of description elements does not match count of keywords!");}

#warning incomplete feature
//   for (auto i = 0; i < N; ++i) {
//
//     addKeyword(keywords[i], valueTypes[i], useMandatory ? mandatory[i] : true);
//
//   }
}
// -------------------------------------------------------------------------- //
void Reader::addKeyword                  (const std::string &                           keyword,
                                          ValueTypeID                                   valueType,
                                          bool                                          mandatory
) {
  auto descriptor = Descriptor(keyword, valueType, mandatory);
  descriptorValidityCheck(descriptor);
  addKeyword(descriptor);
}
// .......................................................................... //
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
  descriptorValidityCheck(descriptor);
  addKeyword(descriptor);
}

// ========================================================================== //
// I/O

std::string Reader::to_string() const {
  std::string reVal = "Parrot::Reader object, ready to extract these objects:\n";

  for (const auto & descriptor : descriptors) {reVal += descriptor.to_string();}

  return reVal;
}
