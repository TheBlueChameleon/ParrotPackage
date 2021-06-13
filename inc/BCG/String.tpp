// ========================================================================= //
// dependencies

// STL
#include <stdexcept>

// ========================================================================== //
// local macro

#define THROWTEXT(msg) (std::string("RUNTIME EXCEPTION IN ") + (__PRETTY_FUNCTION__) + "\n" + msg)

// ========================================================================== //
// procs

// Source:https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring

static inline void BCG::ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .//
static inline void BCG::rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .//
static inline void BCG::trim(std::string &s) {ltrim(s); rtrim(s);}
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .//
static inline std::string BCG::ltrim_copy(std::string s) {ltrim(s); return s;}
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .//
static inline std::string BCG::rtrim_copy(std::string s) {rtrim(s); return s;}
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .//
static inline std::string BCG:: trim_copy(std::string s) {trim(s); return s;}

// ........................................................................ //
static inline void BCG::fullTrim(std::string &s) {
  auto begin = s.begin(),
       end   = s.end();

  for (auto ch = begin; ch != end; ++ch) {
    if (std::isspace(*ch)) {
      s.erase(ch);
      end = s.end();
    }
  }
}
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .//
static inline std::string BCG::fullTrim_copy(std::string s) {fullTrim(s); return s;}

// ------------------------------------------------------------------------ //
// case conversion

static inline void        BCG::to_uppercase(std::string & s) {for (auto & c: s) {c = toupper(c);}          }
static inline std::string BCG::   uppercase(std::string   s) {for (auto & c: s) {c = toupper(c);} return s;}

// ========================================================================== //

#undef THROWTEXT
