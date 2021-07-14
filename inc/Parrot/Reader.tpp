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
  descriptorValidityCheck(descriptor);
  addKeyword(descriptor);
}
// .......................................................................... //

// ========================================================================== //

#undef THROWTEXT
