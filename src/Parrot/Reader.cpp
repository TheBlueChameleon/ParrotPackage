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
// Parsing machinery definitions

// -------------------------------------------------------------------------- //
// Module globals aka state variables, non-exposed

std::string       filename                      ;
std::string       lineOriginal                  ;
std::string       linePreparsed                 ;
std::string       currentKeyword                ;
std::string       defaultValue                  ;
std::string       readValue                     ;
std::vector<bool> foundInFile                   ;
int               linenumber           =      -1;
size_t            keywordID            =      -1;
bool              flagConditionHandled =   false;
bool              verboseFlag          =   false;
FileContent       content                       ;
const Reader *    instancePtr          = nullptr;

// -------------------------------------------------------------------------- //
// parser module local function definitions

std::string parseMessage(std::string message);                                  // returns a copy of <message> with $X replaced with the state string
void        parseLine();                                                        // supervises the parsing process, uses the state variables
void        parseShowState ();                                                  // purely for debug, print state vars to stdout
void        parseResetState(bool fullReset = false);                            // resets the state vars; fullReset also affects file-global states (eg. linenumber)

// partial parsing functions return true if handling the section concludes parsing
bool splitLine();                                                               // finds assignmentMarker and sets keyword and readValue
bool identifyKeyword();                                                         // sets keywordID, handles unexpected keywords
bool duplicateCheck();                                                          // checks whehter keyword was parsed before and informs about handling
bool preparse();                                                                // trimming, case sensitivity, user preparsing; sets defaultValue

// ========================================================================== //
// Private Functions

void Reader::descriptorValidityCheck(const Parrot::Descriptor & descriptor) const {
  auto key = descriptor.getKey();
  if (  key.empty()          ) {throw InvalidDescriptorError(THROWTEXT("    no keyword name specified!"));}
  if ( !keywordCaseSensitive ) {BCG::to_uppercase(key);}
  if (  hasKeyword(key)      ) {throw InvalidDescriptorError(THROWTEXT("    keyword '" + descriptor.getKey() + "' already registered!"));}
}

// ========================================================================== //
// CTors

Reader::Reader(const std::vector<Descriptor> & descriptors) {
  for (const auto & descriptor : descriptors) {addKeyword(descriptor);}
}

// ========================================================================== //
// Getters

char                                    Reader::getCommentMarker        () const {return commentMarker        ;}
char                                    Reader::getMultilineMarker      () const {return multilineMarker      ;}
char                                    Reader::getAssignmentMarker     () const {return assignmentMarker     ;}
bool                                    Reader::getKeywordCaseSensitive () const {return keywordCaseSensitive ;}
bool                                    Reader::getVerbose              () const {return verbose              ;}
// -------------------------------------------------------------------------- //
const MissingKeywordPolicy &            Reader::getMissingKeywordPolicyMandatory  () const {return missingKeywordPolicyMandatory   ;}
const std::string          &            Reader::getMissingKeywordTextMandatory    () const {return missingKeywordTextMandatory     ;}
const MissingKeywordPolicy &            Reader::getMissingKeywordPoliyNonMandatory() const {return missingKeywordPolicyNonMandatory;}
const std::string          &            Reader::getMissingKeywordTextNonMandatory () const {return missingKeywordTextNonMandatory  ;}
const MissingKeywordPolicy &            Reader::getUnexpectedKeywordPolicy        () const {return unexpectedKeywordPolicy         ;}
const std::string          &            Reader::getUnexpectedKeywordText          () const {return unexpectedKeywordText           ;}
const MissingKeywordPolicy &            Reader::getDuplicateKeywordPolicy         () const {return duplicateKeywordPolicy          ;}
const std::string          &            Reader::getDuplicateKeywordText           () const {return duplicateKeywordText            ;}
// -------------------------------------------------------------------------- //
size_t                                  Reader::size            () const {return descriptors.size();}
// .......................................................................... //
bool                                    Reader::hasKeyword      (const std::string & keyword) const {
  auto end = descriptors.end();
  return std::find_if(descriptors.begin(), end,
                      [keyword] (const auto & descriptor) {return descriptor.getKey() == keyword;}
                     ) != end;
}
// .......................................................................... //
size_t                                  Reader::getKeywordIndex (const std::string & keyword) const {
  auto start = descriptors.begin();
  auto end   = descriptors.end();
  auto spot  = std::find_if(start, end,
                            [keyword] (const auto & descriptor) {return descriptor.getKey() == keyword;}
                           );

  if (spot == end) {return std::string::npos;}
  else             {return std::distance(start, spot);}
}
// -------------------------------------------------------------------------- //
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
  commentMarker                    = '#';
  multilineMarker                  = '\\';
  keywordCaseSensitive             = false;
  verbose                          = true;

  missingKeywordPolicyMandatory    = MissingKeywordPolicy::Warning;
  missingKeywordTextMandatory      = "mandatory keyword $K was not found in file $F!";
  missingKeywordPolicyNonMandatory = MissingKeywordPolicy::Exception;
  missingKeywordTextNonMandatory   = "keyword $K was not found; reverting to default ($D)";
  unexpectedKeywordPolicy          = MissingKeywordPolicy::Warning;
  unexpectedKeywordText            = "unexpected keyword in file $F! (Taken as string keyword)\n$L";

  resetKeywords();
}
// .......................................................................... //
void Reader::resetKeywords() {descriptors.clear();}
// -------------------------------------------------------------------------- //
void Reader::setMissingKeywordPolicyMandatory  (const MissingKeywordPolicy & newVal) {missingKeywordPolicyMandatory    = newVal;}
void Reader::setMissingKeywordTextMandatory    (const std::string          & newVal) {missingKeywordTextMandatory      = newVal;}
void Reader::setMissingKeywordPoliyNonMandatory(const MissingKeywordPolicy & newVal) {missingKeywordPolicyNonMandatory = newVal;}
void Reader::setMissingKeywordTextNonMandatory (const std::string          & newVal) {missingKeywordTextNonMandatory   = newVal;}
void Reader::setUnexpectedKeywordPolicy        (const MissingKeywordPolicy & newVal) {unexpectedKeywordPolicy          = newVal;}
void Reader::setUnexpectedKeywordText          (const std::string          & newVal) {unexpectedKeywordText            = newVal;}
void Reader::setDuplicateKeywordPolicy         (const MissingKeywordPolicy & newVal) {duplicateKeywordPolicy           = newVal;}
void Reader::setDuplicateKeywordText           (const std::string          & newVal) {duplicateKeywordText             = newVal;}
// -------------------------------------------------------------------------- //
void Reader::setCommentMarker                  (char                         newVal) {commentMarker         = newVal;}
void Reader::setMultilineMarker                (char                         newVal) {multilineMarker       = newVal;}
void Reader::setAssignmentMarker               (char                         newVal) {assignmentMarker      = newVal;}
void Reader::setKeywordCaseSensitive           (bool                         newVal) {keywordCaseSensitive  = newVal;}
void Reader::setVerbose                        (bool                         newVal) {verbose               = newVal;}
// -------------------------------------------------------------------------- //
void Reader::addKeyword                  (const std::string &                           keyword,
                                          ValueTypeID                                   valueType,
                                          bool                                          mandatory
) {
  auto descriptor = Descriptor(keyword, valueType, mandatory);
  addKeyword(descriptor);
}
// .......................................................................... //
void Reader::addKeyword                  (const std::string &                           keyword,
                                          const std::any &                              defaultValue,
                                          bool                                          mandatory
) {
  auto descriptor = Descriptor(keyword, defaultValue, mandatory);
  addKeyword(descriptor);
}
// .......................................................................... //
void Reader::addKeyword                  (const Parrot::Reader::MinimalDescriptor & descriptor) {
  auto descriptor_full = Parrot::Descriptor(
    std::get<0>(descriptor),
    std::get<1>(descriptor),
    std::get<2>(descriptor)
  );
  addKeyword(descriptor_full);
}
// .......................................................................... //
void Reader::addKeyword                  (const             Parrot::Descriptor  & descriptor ) {
  descriptorValidityCheck(descriptor);
  if (!keywordCaseSensitive) {
    auto descriptorCopy = descriptor;
    descriptorCopy.setKey( BCG::uppercase(descriptor.getKey()) );
    descriptors.push_back(descriptorCopy);

  } else {
    descriptors.push_back(descriptor);
  }
}
// -------------------------------------------------------------------------- //
void Reader::addKeywords                 (const std::vector<Parrot::Descriptor> & descriptors) {
  std::cout << "*** via full descriptor list" << std::endl;
  for (auto & descriptor : descriptors) {
    descriptorValidityCheck(descriptor);
    addKeyword(descriptor);
  }
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

Parrot::FileContent Reader::operator() (const std::string & source) const {
  std::fstream hFile = BCG::openThrow(source, std::fstream::in);

  parseResetState(true);
  content            = FileContent(source);
  filename           = source;
  foundInFile        = std::vector<bool> ( descriptors.size() );
  instancePtr        = this;
  verboseFlag        = this->verbose;

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
    parseResetState();
  }


  if (verbose) {
    std::cout << "\nCompleted parsing file '" << source<< "' (" << linenumber << " lines)" << std::endl << std::endl;
    std::cout << to_string() << std::endl;
  }

  return content;
}
// -------------------------------------------------------------------------- //
std::string Reader::to_string() const {
  std::string reVal = "Parrot::Reader object\n";
  reVal += "  comment marker                           : "s + (commentMarker         ? std::string(1, commentMarker  ) : "(none)"s) + "\n";
  reVal += "  line continuation marker                 : "s + (multilineMarker       ? std::string(1, multilineMarker) : "(none)"s) + "\n";
  reVal += "  assignment marker                        : "s + (assignmentMarker                                                   ) + "\n";
  reVal += "  treat keywords case sensitively          : "s + (keywordCaseSensitive  ?                           "yes" : "no"     ) + "\n";
  reVal += "  verbose mode                             : "s + (verbose               ?                           "yes" : "no"     ) + "\n";

  reVal += "  policy for missing non-mandatory keywords: " + Parrot::missingKeywordPolicyName(missingKeywordPolicyNonMandatory) + "\n";
  reVal += "    message                                : " + missingKeywordTextNonMandatory + "\n";
  reVal += "  policy for missing mandatory keywords    : " + Parrot::missingKeywordPolicyName(missingKeywordPolicyMandatory) + "\n";
  reVal += "    message                                : " + missingKeywordTextMandatory + "\n";
  reVal += "  policy for unexpected keywords           : " + Parrot::missingKeywordPolicyName(unexpectedKeywordPolicy) + "\n";
  reVal += "    message                                : " + unexpectedKeywordText + "\n";
  reVal += "  policy for duplicate keywords            : " + Parrot::missingKeywordPolicyName(duplicateKeywordPolicy) + "\n";
  reVal += "    message                                : " + duplicateKeywordText + "\n";


  reVal += "ready to extract these objects:\n";

  for (const auto & descriptor : descriptors) {reVal += descriptor.to_string();}

  return reVal;
}


// ========================================================================== //
// Parsing Machinery implementation

std::string parseMessage(std::string message) {
  /* *text variables
   * $F -- file               : filename as specified in call operator (via content)
   * $L -- line               : line as read from file, trimmed        (lineOriginal)
   * $# -- linenumber         : current line number in parsing process (linenumber)
   * $K -- keyword            : current keyword                        (keyword)
   * $D -- default value      : default value of given keyword         (defaultValue)
   * $V -- value (read value) : parsed value as string                 (readValue)
   */

  BCG::replaceAll(message, "$F", filename);
  BCG::replaceAll(message, "$L", lineOriginal);
  BCG::replaceAll(message, "$#", std::to_string(linenumber));
  BCG::replaceAll(message, "$K", currentKeyword);
  BCG::replaceAll(message, "$D", defaultValue);
  BCG::replaceAll(message, "$V", readValue);

  return message;
}
// -------------------------------------------------------------------------- //
void parseLine() {
  // no parsing criteria: empty or comment

  if ( linePreparsed.empty()                               ) {return;}
  if ( linePreparsed[0] == instancePtr->getCommentMarker() ) {return;}

  // ........................................................................ //
  // partial parsers

  if ( splitLine      () ) {return;}
  if ( identifyKeyword() ) {return;}
  if ( duplicateCheck () ) {return;}
  if ( preparse       () ) {return;}

#warning dummy active
  content.addElement(currentKeyword, readValue, true, flagConditionHandled);
}
// .......................................................................... //
void parseResetState(bool fullReset) {
  lineOriginal           .clear() ;
  linePreparsed          .clear() ;
  currentKeyword         .clear() ;
  defaultValue           .clear() ;
  readValue              .clear() ;
  keywordID              =      -1;

  if (fullReset) {
    filename             .clear() ;
    foundInFile          .clear() ;
    linenumber           =      -1;
    flagConditionHandled =   false;
    verboseFlag          =   false;
    content              .reset() ;
    instancePtr          = nullptr;
  }
}
// .......................................................................... //
void parseShowState () {
  std::cout << "filename            " << filename                           << std::endl;
  std::cout << "lineOriginal        " << lineOriginal                       << std::endl;
  std::cout << "linePreparsed       " << linePreparsed                      << std::endl;
  std::cout << "keyword             " << currentKeyword                     << std::endl;
  std::cout << "defaultValue        " << defaultValue                       << std::endl;
  std::cout << "readValue           " << readValue                          << std::endl;
  std::cout << "foundInFile         " << BCG::vector_to_string(foundInFile) << std::endl;
  std::cout << "linenumber          " << linenumber                         << std::endl;
  std::cout << "keywordID           " << keywordID                          << std::endl;
  std::cout << "flagConditionHandled" << flagConditionHandled               << std::endl;
  std::cout << "verboseFlag         " << verboseFlag                        << std::endl;
//   std::cout << "content             " << content                            << std::endl;
  std::cout << "instancePtr         " << instancePtr                        << std::endl;
  std::cout << std::string(80, '~') << std::endl;
}
// -------------------------------------------------------------------------- //
bool splitLine() {
  auto separationIdx = linePreparsed.find('=');

  if (separationIdx == std::string::npos) {
    if (verboseFlag) {
      BCG::writeWarning("found no value in line " + std::to_string(linenumber) + ":\n" +
                        lineOriginal
      );
    }
    return true;
  }

  currentKeyword  = linePreparsed.substr(0, separationIdx);
  readValue       = linePreparsed.substr(separationIdx + 1, std::string::npos);
  BCG::trim(currentKeyword);
  if ( !instancePtr->getKeywordCaseSensitive() ) {BCG::to_uppercase(currentKeyword);}
  return false;
}
// .......................................................................... //
bool identifyKeyword() {
  keywordID = instancePtr->getKeywordIndex(currentKeyword);
  bool update = false;

  if ( keywordID == std::string::npos ) {
    switch ( instancePtr->getUnexpectedKeywordPolicy() ) {
      case MissingKeywordPolicy::Ignore :
        return true;

      case MissingKeywordPolicy::Silent :
        flagConditionHandled = true;
        update               = true;
        break;

      case MissingKeywordPolicy::Warning :
        flagConditionHandled = true;
        update               = true;
        BCG::writeWarning( parseMessage(instancePtr->getUnexpectedKeywordText()) );
        break;

      case MissingKeywordPolicy::Exception :
        throw MissingKeywordError(THROWTEXT(
          parseMessage( instancePtr->getUnexpectedKeywordText() )
          ));
        break;
    }
  }

  if (update) {
    BCG::trim(readValue);
    content.addElement(currentKeyword, readValue, false, flagConditionHandled);
    return true;
  }

  return false;
}
// .......................................................................... //
bool duplicateCheck() {
  bool update = false;

  if (foundInFile[keywordID]) {
    switch ( instancePtr->getDuplicateKeywordPolicy() ) {
      case MissingKeywordPolicy::Ignore :
        return true;

      case MissingKeywordPolicy::Silent :
        flagConditionHandled = true;
        update               = true;
        break;

      case MissingKeywordPolicy::Warning :
        flagConditionHandled = true;
        update               = true;
        BCG::writeWarning( parseMessage( instancePtr->getDuplicateKeywordText() ) );
        break;

      case MissingKeywordPolicy::Exception :
        throw MissingKeywordError(THROWTEXT(
          parseMessage( instancePtr->getDuplicateKeywordText() )
        ));
        break;
    }

  } else {
    foundInFile[keywordID] = true;
  }

  if (update) {
    BCG::trim(readValue);
    content.updateElement(currentKeyword, readValue, false, flagConditionHandled);
    return true;
  }

  return false;
}
// .......................................................................... //
bool preparse() {
  auto descriptor = instancePtr->getDescriptor(keywordID);
  defaultValue    = getAnyText( descriptor.getValue() );

  if ( descriptor.isTrimLeadingWhitespaces () ) {BCG::ltrim       (readValue);}
  if ( descriptor.isTrimTrailingWhitespaces() ) {BCG::rtrim       (readValue);}
  if ( descriptor.isCaseSensitive          () ) {BCG::to_uppercase(readValue);}
  if ( descriptor.getUserPreParser         () ) {readValue = descriptor.getUserPreParser()(readValue);}

  parseShowState();

  return false;
}
