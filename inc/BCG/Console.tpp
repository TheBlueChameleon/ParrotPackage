// ========================================================================= //
// dependencies

// STL
#include <stdexcept>

// ========================================================================== //
// local macro

#define THROWTEXT(msg) (std::string("RUNTIME EXCEPTION IN ") + (__PRETTY_FUNCTION__) + "\n" + msg)

// ========================================================================== //


static inline void BCG::consoleClear()                               {if (isTTY) {std::cout << "\033[H\033[J";}}
static inline void BCG::consoleGotoRC(const int row, const int col)  {if (isTTY) {std::cout << "\033[" << row << ";" << col << "H";}}

// ========================================================================== //

#undef THROWTEXT
