// ========================================================================= //
// dependencies

// STL
#include <stdexcept>

#include <iostream>

// own
#include "BCG.hpp"

// ========================================================================== //
// local macro

#define THROWTEXT(msg) (std::string("RUNTIME EXCEPTION IN ") + (__PRETTY_FUNCTION__) + "\n" + msg)

// ========================================================================== //
// procs

using namespace BCG;

std::vector<std::string> BCG::splitString(const std::string & s, const char separator) {
  std::vector<std::string> reVal;
  if ( s.empty() ) {return reVal;}

  auto posSeparator = s.find(separator);
  auto last = 0u;

  while (posSeparator != std::string::npos) {
    reVal.push_back( s.substr(last, posSeparator - last) );

    last = posSeparator + 1;
    posSeparator = s.find(separator, posSeparator + 1);
  }

  if (last != s.size()) {
    reVal.push_back( s.substr(last, s.size() - last) );
  }

  return reVal;
}
// .......................................................................... //
bool BCG::wildcardmatch(const char *first, const char * second) {
  // https://www.geeksforgeeks.org/wildcard-character-matching/

  if (*first == '\0' && *second == '\0')                        { return true ; }
  if (*first == '*'  && *(first+1) != '\0' && *second == '\0')  { return false; }
  if (*first == '?'  || *first == *second)                      { return wildcardmatch(first+1, second+1); }
  if (*first == '*')                                            { return wildcardmatch(first+1, second) || wildcardmatch(first, second+1); }
  return false;
}
bool BCG::wildcardmatch(const std::string & pattern, const std::string & searchstring) {return wildcardmatch(pattern.c_str(), searchstring.c_str());}
