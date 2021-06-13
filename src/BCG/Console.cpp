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
// globals

namespace BCG {
  bool   isTTY = true;
}

// ========================================================================== //
// procs

using namespace BCG;

void BCG::consoleSetcolor (const ConsoleColors code) {
  if (!isTTY) {return;}

  switch (code) {
    case ConsoleColors::FORE_BLACK         : std::cout << ("\x1b[30m") ; break;
    case ConsoleColors::FORE_RED           : std::cout << ("\x1b[31m") ; break;
    case ConsoleColors::FORE_GREEN         : std::cout << ("\x1b[32m") ; break;
    case ConsoleColors::FORE_BROWN         : std::cout << ("\x1b[33m") ; break;
    case ConsoleColors::FORE_BLUE          : std::cout << ("\x1b[34m") ; break;
    case ConsoleColors::FORE_PURPLE        : std::cout << ("\x1b[35m") ; break;
    case ConsoleColors::FORE_CYAN          : std::cout << ("\x1b[36m") ; break;
    case ConsoleColors::FORE_DARK_GREY     : std::cout << ("\x1b[37m") ; break;
    case ConsoleColors::FORE_NORMAL        : std::cout << ("\x1b[39m") ; break;
    case ConsoleColors::FORE_BRIGHT_GREY   : std::cout << ("\x1b[90m") ; break;
    case ConsoleColors::FORE_BRIGHT_RED    : std::cout << ("\x1b[91m") ; break;
    case ConsoleColors::FORE_BRIGHT_GREEN  : std::cout << ("\x1b[92m") ; break;
    case ConsoleColors::FORE_YELLOW        : std::cout << ("\x1b[93m") ; break;
    case ConsoleColors::FORE_BRIGHT_BLUE   : std::cout << ("\x1b[94m") ; break;
    case ConsoleColors::FORE_PINK          : std::cout << ("\x1b[95m") ; break;
    case ConsoleColors::FORE_BRIGHT_CYAN   : std::cout << ("\x1b[96m") ; break;
    case ConsoleColors::FORE_WHITE         : std::cout << ("\x1b[97m") ; break;

    case ConsoleColors::BACK_BLACK         : std::cout << ("\x1b[40m") ; break;
    case ConsoleColors::BACK_RED           : std::cout << ("\x1b[41m") ; break;
    case ConsoleColors::BACK_GREEN         : std::cout << ("\x1b[42m") ; break;
    case ConsoleColors::BACK_BROWN         : std::cout << ("\x1b[43m") ; break;
    case ConsoleColors::BACK_BLUE          : std::cout << ("\x1b[44m") ; break;
    case ConsoleColors::BACK_PURPLE        : std::cout << ("\x1b[45m") ; break;
    case ConsoleColors::BACK_CYAN          : std::cout << ("\x1b[46m") ; break;
    case ConsoleColors::BACK_DARK_GREY     : std::cout << ("\x1b[47m") ; break;
    case ConsoleColors::BACK_NORMAL        : std::cout << ("\x1b[49m") ; break;
    case ConsoleColors::BACK_BRIGHT_GREY   : std::cout << ("\x1b[100m"); break;
    case ConsoleColors::BACK_BRIGHT_RED    : std::cout << ("\x1b[101m"); break;
    case ConsoleColors::BACK_BRIGHT_GREEN  : std::cout << ("\x1b[102m"); break;
    case ConsoleColors::BACK_YELLOW        : std::cout << ("\x1b[103m"); break;
    case ConsoleColors::BACK_BRIGHT_BLUE   : std::cout << ("\x1b[104m"); break;
    case ConsoleColors::BACK_PINK          : std::cout << ("\x1b[105m"); break;
    case ConsoleColors::BACK_BRIGHT_CYAN   : std::cout << ("\x1b[106m"); break;
    case ConsoleColors::BACK_WHITE         : std::cout << ("\x1b[107m"); break;

    // These do not work on all console programs.
    case ConsoleColors::SPC_NORMAL         : std::cout << ("\x1b[0m")  ; break;
    case ConsoleColors::SPC_ITALICS_ON     : std::cout << ("\x1b[3m")  ; break;
    case ConsoleColors::SPC_ITALICS_OFF    : std::cout << ("\x1b[23m") ; break;
    case ConsoleColors::SPC_UNDERLINE_ON   : std::cout << ("\x1b[4m")  ; break;
    case ConsoleColors::SPC_UNDERLINE_OFF  : std::cout << ("\x1b[24m") ; break;
    case ConsoleColors::SPC_BLINK_ON       : std::cout << ("\x1b[5m")  ; break;
    case ConsoleColors::SPC_BLINK_OFF      : std::cout << ("\x1b[25m") ; break;
    case ConsoleColors::SPC_BOLD_ON        : std::cout << ("\x1b[1m")  ; break;
    case ConsoleColors::SPC_BOLD_OFF       : std::cout << ("\x1b[21m") ; break;
  }
}

// .......................................................................... //

void BCG::writeWarning (const std::string & text,
                   const std::string & headline,
                   const ConsoleColorsTriple &     textColors,
                   const ConsoleColorsTriple & headlineColors,
                   const int indentFirst,
                   const int indentHanging,
                   std::ostream & stream
) {

  if ( (indentFirst < 0) || (indentHanging < 0) ) {
    throw std::invalid_argument(THROWTEXT(
      "    indent parameters needs to be greater than zero!"
    ));
  }

  consoleSetcolor(headlineColors.spc );
  consoleSetcolor(headlineColors.fore);
  consoleSetcolor(headlineColors.back);

  stream << std::string(indentFirst, ' ');
  stream << headline << std::endl;


  consoleSetcolor(textColors.spc );
  consoleSetcolor(textColors.fore);
  consoleSetcolor(textColors.back);

  int indentTotal = indentFirst + indentHanging;
  const std::string indent(indentTotal, ' ');

  auto lines = splitString(text, '\n');
  for (auto & line : lines) {
    stream << indent;
    stream << line;
    stream << std::endl;
  }

  consoleSetcolor(ConsoleColors::SPC_NORMAL);
}
// .......................................................................... //
void BCG::writeBoxed(const std::string & text,
                const ConsoleColorsTriple & format,
                const int  width,
                const char vertical,
                const char horizontal,
                const char edge,
                std::ostream & stream
) {
  if (width < 4) {
    throw std::runtime_error(THROWTEXT("    width must be greater than 4."));
  }

  auto lines = splitString(text, '\n');

  consoleSetcolor(format.spc );
  consoleSetcolor(format.fore);
  consoleSetcolor(format.back);

  const std::string deco = std::string(1, edge) + std::string(width - 2, vertical) + std::string(1, edge);

  stream << deco << '\n';
  for (auto & line : lines) {
    stream << horizontal << ' ';
    stream << line;
    stream << std::string(width - 4 - line.size(), ' ');
    stream << ' ' << horizontal << '\n';
  }
  stream << deco << std::endl;

  consoleSetcolor(ConsoleColors::SPC_NORMAL);
}
// ------------------------------------------------------------------------ //
void BCG::idleAnimation(const std::string & text) {
  if (!isTTY) {return;}

  static int phase = 0;

  std::cout << text;

  switch (phase) {
    case 0 : std::cout << "-"; break;
    case 1 : std::cout << "\\"; break;
    case 2 : std::cout << "|"; break;
    case 3 : std::cout << "/"; break;
  }
  std::cout << "\r" << std::flush;

  phase = (phase + 1) % 4;
}
