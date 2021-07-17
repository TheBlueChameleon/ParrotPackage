// ========================================================================= //
// dependencies
// STL
#include <stdexcept>

#include <vector>
#include <any>
#include <initializer_list>

// own
#include "Parrot/Definitions.hpp"

// ========================================================================== //
// local macro

#define THROWTEXT(msg) ("RUNTIME EXCEPTION IN "s + (__PRETTY_FUNCTION__) + "\n"s + msg)

// ========================================================================== //
// Setters

template <typename T>
void Parrot::Reader::addKeyword  (const std::string &                           keyword,
                                  const T &                                     defaultValue,
                                  bool                                          mandatory
) {
  auto descriptor = Descriptor(keyword, defaultValue, mandatory);
  addKeyword(descriptor);
}
// -------------------------------------------------------------------------- //
template <typename T>
void Parrot::Reader::addKeywordRanged            (const std::string &                           keyword,
                                                  const T &                                     defaultValue,
                                                  PARROT_TYPE(ValueTypeID::Real)                min,
                                                  PARROT_TYPE(ValueTypeID::Real)                max,
                                                  RestrictionViolationPolicy                    policy,
                                                  const std::string &                           restrictionViolationText,
                                                  bool                                          mandatory
) {
  auto descriptor = Descriptor();
  descriptor.makeRanged(keyword, defaultValue, min, max, policy, restrictionViolationText, mandatory);
  descriptorValidityCheck (descriptor);
  addKeyword              (descriptor);
}
// .......................................................................... //
template <typename T>
void Parrot::Reader::addKeywordListboundPreParse (const std::string &                           keyword,
                                                  const T &                                     defaultValue,
                                                  const PARROT_TYPE(ValueTypeID::StringList) &  list,
                                                  bool                                          forbiddenList,
                                                  RestrictionViolationPolicy                    policy,
                                                  const std::string &                           restrictionViolationText,
                                                  bool                                          mandatory
) {
  auto descriptor = Descriptor();
  descriptor.makeListboundPreParse(keyword, defaultValue, list, forbiddenList, policy, restrictionViolationText, mandatory);
  descriptorValidityCheck (descriptor);
  addKeyword              (descriptor);
}
// .......................................................................... //
template <typename LT>
void Parrot::Reader::addKeywordListboundAftParse (const std::string &                           keyword,
                                                  ValueTypeID                                   valueType,
                                                  const std::vector<LT> &                       list,
                                                  bool                                          forbiddenList,
                                                  RestrictionViolationPolicy                    policy,
                                                  const std::string &                           restrictionViolationText,
                                                  bool                                          mandatory
) {
  auto descriptor = Descriptor();
  descriptor.makeListboundAftParse(keyword, valueType, list, forbiddenList, policy, restrictionViolationText, mandatory);
  descriptorValidityCheck (descriptor);
  addKeyword              (descriptor);
}
// .......................................................................... //
template <typename DT, typename LT>
void Parrot::Reader::addKeywordListboundAftParse (const std::string &                           keyword,
                                                  const DT &                                    defaultValue,
                                                  const std::vector<LT> &                       list,
                                                  bool                                          forbiddenList,
                                                  RestrictionViolationPolicy                    policy,
                                                  const std::string &                           restrictionViolationText,
                                                  bool                                          mandatory
) {
  auto descriptor = Descriptor();
  descriptor.makeListboundAftParse(keyword, defaultValue, list, forbiddenList, policy, restrictionViolationText, mandatory);
  descriptorValidityCheck (descriptor);
  addKeyword              (descriptor);
}
// .......................................................................... //
template <typename T>
void Parrot::Reader::addKeywordUserboundPreParse (const std::string &                           keyword,
                                                  const T &                                     defaultValue,
                                                  const std::function< bool(const PARROT_TYPE(ValueTypeID::String) &)> &uFunc,
                                                  RestrictionViolationPolicy                    policy,
                                                  const std::string &                           restrictionViolationText,
                                                  bool                                          mandatory
) {
  auto descriptor = Descriptor();
  descriptor.makeUserboundPreParse(keyword, defaultValue, uFunc, policy, restrictionViolationText, mandatory);
  descriptorValidityCheck (descriptor);
  addKeyword              (descriptor);
}
// .......................................................................... //
template <typename T>
void Parrot::Reader::addKeywordUserboundAftParse (const std::string &                           keyword,
                                                  ValueTypeID                                   valueType,
                                                  const std::function<bool (const T &)> &       uFunc,
                                                  RestrictionViolationPolicy                    policy,
                                                  const std::string &                           restrictionViolationText,
                                                  bool                                          mandatory
) {
  auto descriptor = Descriptor();
  descriptor.makeUserboundAftParse(keyword, valueType, uFunc, policy, restrictionViolationText, mandatory);
  descriptorValidityCheck (descriptor);
  addKeyword              (descriptor);
}
// .......................................................................... //
template <typename T>
void Parrot::Reader::addKeywordUserboundAftParse (const std::string &                           keyword,
                                                  const T &                                     defaultValue,
                                                  const std::function<bool (const T &)> &       uFunc,
                                                  RestrictionViolationPolicy                    policy,
                                                  const std::string &                           restrictionViolationText,
                                                  bool                                          mandatory
) {
  auto descriptor = Descriptor();
  descriptor.makeUserboundAftParse(keyword, defaultValue, uFunc, policy, restrictionViolationText, mandatory);
  descriptorValidityCheck (descriptor);
  addKeyword              (descriptor);
}

// ========================================================================== //

#undef THROWTEXT
