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
// CTors

Reader::Reader(const std::vector<Parrot::Descriptor> & descriptors) :
  descriptors(descriptors)
{}

// ========================================================================== //
// Getters

char                                    Reader::getCommentMarker  () const {return commentMarker;}
char                                    Reader::getMultilineMarker() const {return multilineMarker;}
bool                                    Reader::getVerbose        () const {return verbose;}
// -------------------------------------------------------------------------- //
const MissingKeywordPolicy &            Reader::getMissingKeywordPolicyMandatory  () const {return missingKeywordPolicyMandatory  ;}
const std::string          &            Reader::getMissingKeywordTextMandatory    () const {return missingKeywordTextMandatory    ;}
const MissingKeywordPolicy &            Reader::getMissingKeywordPoliyNonMandatory() const {return missingKeywordPoliyNonMandatory;}
const std::string          &            Reader::getMissingKeywordTextNonMandatory () const {return missingKeywordTextNonMandatory ;}
// -------------------------------------------------------------------------- //
size_t                                  Reader::size      () const {return descriptors.size();}
bool                                    Reader::hasKeyword(const std::string & keyword) const {
  auto end = descriptors.end();
  return std::find_if(descriptors.begin(), end, [keyword] (const auto & descriptor) {return descriptor.getKey() == keyword;}) != end;
}
// .......................................................................... //
const std::vector<Parrot::Descriptor> & Reader::getDescriptors()                            const {return descriptors;}
const             Parrot::Descriptor  & Reader::getDescriptor (const size_t        idx    ) const {
  if (idx >= descriptors.size()) {throw std::runtime_error(THROWTEXT("    index out of bounds!"));}
  return descriptors[idx];
}
const             Parrot::Descriptor  & Reader::getDescriptor (const std::string & keyword) const {
  auto end  = descriptors.end();
  auto spot = std::find_if(descriptors.begin(), end, [keyword] (const auto & descriptor) {return descriptor.getKey() == keyword;});

  if (spot == end) {throw std::runtime_error(THROWTEXT("    keyword does not exist!"));}

  return *spot;
}
