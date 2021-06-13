// ========================================================================= //
// dependencies

// STL
#include <stdexcept>

// ========================================================================== //
// local macro

#define THROWTEXT(msg) (std::string("RUNTIME EXCEPTION IN ") + (__PRETTY_FUNCTION__) + "\n" + msg)

// ========================================================================== //

namespace BCG {
  static inline void consoleClear()                               {if (isTTY) {std::cout << "\033[H\033[J";}}
  static inline void consoleGotoRC(const int row, const int col)  {if (isTTY) {std::cout << "\033[" << row << ";" << col << "H";}}
}

// ========================================================================== //

#undef THROWTEXT
