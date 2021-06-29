/**
 * @defgroup BCG_Console BCG Console Output Module
 *
 * @brief convenience functions to work with a TTY
 */

#ifndef BCG_CONSOLE_HPP
#define BCG_CONSOLE_HPP

// ========================================================================= //
// dependencies

#include <stdexcept>
#include <iostream>

// ========================================================================== //

namespace BCG {
  //! @addtogroup BCG_Console
  //! @{

  // ------------------------------------------------------------------------ //
  // enums and structs

  /**
   * @brief Lists all the format specifiers compatible with common linux consoles.
   *  Use with a ConsoleColorsTriple
   */
  enum class ConsoleColors {
    FORE_BLACK,
    FORE_RED,
    FORE_GREEN,
    FORE_BROWN,
    FORE_BLUE,
    FORE_PURPLE,
    FORE_CYAN,
    FORE_DARK_GREY,
    FORE_NORMAL,
    FORE_BRIGHT_GREY,
    FORE_BRIGHT_RED,
    FORE_BRIGHT_GREEN,
    FORE_YELLOW,
    FORE_BRIGHT_BLUE,
    FORE_PINK,
    FORE_BRIGHT_CYAN,
    FORE_WHITE,

    BACK_BLACK,
    BACK_RED,
    BACK_GREEN,
    BACK_BROWN,
    BACK_BLUE,
    BACK_PURPLE,
    BACK_CYAN,
    BACK_DARK_GREY,
    BACK_NORMAL,
    BACK_BRIGHT_GREY,
    BACK_BRIGHT_RED,
    BACK_BRIGHT_GREEN,
    BACK_YELLOW,
    BACK_BRIGHT_BLUE,
    BACK_PINK,
    BACK_BRIGHT_CYAN,
    BACK_WHITE,

    SPC_NORMAL,
    SPC_UNDERLINE_ON,
    SPC_UNDERLINE_OFF,
    SPC_ITALICS_ON,
    SPC_ITALICS_OFF,
    SPC_BLINK_ON,
    SPC_BLINK_OFF,
    SPC_BOLD_ON,
    SPC_BOLD_OFF,
  };

  /**
   * @brief A specification for a console text format. Many of functions in
   *  @subpage BCG_Console accept an optional parameter of this type, specifying
   *  the output colours.
   *
   * @param fore color attribute of the characters themselves. Must be one of the FORE_* values
   * @param back color attribute of the character background. Must be one of the BACK_* values
   * @param spc  special format attribute for the characters themselves. Must be one of the SPC_* values of ConsoleColors
   *
   * Note that the three slots have an \em intended meaning, but can actually be
   *  used interchangeably; there's nothing stopping you from setting the member
   *  \c back to \c BCG::ConsoleColors::SPC_UNDERLINE_ON and the effect is just
   *  the same as if you had set the member \c spc to that value. Indeed, if,
   *  for example, multiple formats are to be applied while keeping the current
   *  console background colour, this is a valid way of doing so.
   *
   * While there are 2 times 16 colors defined (fore and background), not all
   * consoles support all of them. Be prepared to find the output to be modulo 8
   * (i.e. that there will be no bright versions of the colours).
   * Likewise, it is not guaranteed that the special formats (prefix SPC_) are
   * respected by your console.
   * Also, the interpretation of the \c SPC_ codes varies from terminal to
   * terminal. It is common to find the \c SPC_BOLD_ON switch to activate high
   * intensity colours instead of changing the \em shape of the letters.
   */
  struct ConsoleColorsTriple {
    ConsoleColors fore = ConsoleColors::FORE_WHITE;
    ConsoleColors back = ConsoleColors::BACK_BLACK;
    ConsoleColors spc  = ConsoleColors::SPC_NORMAL;
  };

  // ------------------------------------------------------------------------ //
  // globals

  /**
   * @brief indicates if stdout is a terminal. Needs to be initialized by init()
   *
   * If this is false, then the ConsoleColorsTriple parameters are ignored as
   * well as the consoleSetcolor, consoleClear and consoleGotoRC commands.
   *
   * By default, this value is \c true. init() calls the POSIX function
   * isatty on stdout to determine whether color console features are available.
   */
  extern    bool   isTTY;

  // ------------------------------------------------------------------------ //
  // proc

  /**
   * @brief clears the screen.
   *  Does nothing if <tt>BCG::isTTY</tt> is set to false
   */
  static inline void consoleClear();

  /**
   * @brief places the cursor in (row, col).
   *  Does nothing if <tt>BCG::isTTY</tt> is set to false
   *
   * @param row the row where the console cursor should be placed
   * @param col the column where the console cursor should be placed
   */
  static inline void consoleGotoRC(const int row, const int col);

  /**
   * @brief changes the output format for subsequent outputs to stdout.
   *  Does nothing if <tt>BCG::isTTY</tt> is set to false
   */
  void consoleSetcolor (const ConsoleColors code);

  /**
   * @brief puts a warning message on \c stream
   *
   * @param text the text below the headline. May contain instances of \c \\n
   *  to indicate line breaks in the warning message.
   * @param headline the first line of the warning which will be formatted
   *  differently from the rest of the text
   * @param textColors the output format specification for \c text
   * @param headlineColors the output format specification for \c headline
   * @param indentFirst the number of whitespaces to be printed before \e all
   *  lines of the output
   * @param indentHanging the number of whitespaces to be printed before the
   *  text lines, <em>in addition</em> to the \c indentFirst whitespaces that apply
   *  to all lines.
   * @param stream a \c std::ofstream that designates the device onto which the
   *  output warning should be written.
   *
   * \b Example:
   * @code
   * writeWarning("no value specified\ndefaulting to zero")
   * @endcode
   * produces this output:
   * @code
   * Warning
   *    no value specified
   *    defaulting to zero
   * @endcode
   */
  void writeWarning ( const std::string & text,
                      const std::string & headline = "Warning",
                      const ConsoleColorsTriple & textColors     = {ConsoleColors::FORE_NORMAL},
                      const ConsoleColorsTriple & headlineColors = {ConsoleColors::FORE_BRIGHT_RED},
                      const int indentFirst   = 0,
                      const int indentHanging = 3,
                      std::ostream & stream = std::cerr
  );

  /**
   * @brief puts text in a box and writes it to a stream.
   *
   * @param text the text to be written.<br>
   *  The text should be less than <tt>width - 4</tt> characters wide.
   * @param format the color format to be applied (ignored if
   *  <tt>BCG::isTTY</tt> is set to \c false)
   * @param width the width of the box in characters
   * @param vertical the character used for the vertical boundaries of the box
   * @param horizontal the character used for the horizontal boundaries of the
   *   box
   * @param edge the character to represent the edges of the box
   * @param stream a \c std::ofstream that designates the device onto which the
   *  output warning should be written
   *
   * \b Example:
   * @code
   * writeBoxed("UNIT TEST")
   * @endcode
   * produces this output:
   * @code
   * #==============================================================================#
   * # UNIT TEST                                                                    #
   * #==============================================================================#
   * @endcode
   */
  void writeBoxed(const std::string & text,
                  const ConsoleColorsTriple & format = {ConsoleColors::FORE_WHITE, ConsoleColors::BACK_BLACK, ConsoleColors::SPC_BOLD_ON},
                  const int  width      = 80,
                  const char vertical   = '-',
                  const char horizontal = '|',
                  const char edge       = '+',
                  std::ostream & stream = std::cout
  );

  /**
   * @brief Displays subsequent frames of an idle animation on std::cout.
   *  Does nothing if <tt>BCG::isTTY</tt> is set to false
   *
   * This assumes that the cursor is at the beginning of a new line.
   *
   * The frames of the animation are -, \\, | and /, in that order.
   *
   * @param text a short text to be displayed in front of the animation.
   *  must fit in a single line.
   */
  void idleAnimation(const std::string & text = "please be patient");

  /**
   * @brief writes a vertical bar with evenly spaced tickmarks as well as labels
   *
   * @param width the width of the the scale in characters
   * @param stops the number of tickmarks (including the first, excluding the
   *  last) to fit on the bar.<br>
   *  If the space between two tickmarks drops to two, no tickmarks are written
   *  at all.<br>
   *  Setting this to \c -1 makes writeScale decide itself how many stops to put
   *  on the scale. The intended number of tickmarks is such that there are 5
   *  characters between two tickmarks.
   * @param format the color format to be applied (ignored if
   *  <tt>BCG::isTTY</tt> is set to \c false)
   * @param stream a \c std::ofstream that designates the device onto which the
   *  output warning should be written
   *
   * \b Example:
   * @code
   * BCG::writeScale(80, 10);
   * @endcode
   * produces
   * @code
   *       10      20      30      40      50      60      70      80      90
   * +------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
   * @endcode
   *
   */
  void writeScale(const int width = 80, int stops = -1,
                  const ConsoleColorsTriple & format = {ConsoleColors::SPC_NORMAL},
                  std::ostream & stream = std::cout
  );

  /**
   * @brief overwrites the current line with a string of characters, indicating
   *  current progress.
   *
   * @param percent a value between 0 and 1, indicating current progress
   * @param width   the number of characters to be used for the progress bar
   * @param block   the character to draw the progress bar with
   * @param format the color format to be applied (ignored if
   *  <tt>BCG::isTTY</tt> is set to \c false)
   * @param stream a \c std::ofstream that designates the device onto which the
   *  output warning should be written
   *
   * \b Example:
   * @code
   * BCG::writeScale(80, 10);
   * BCG::updateProgressBar(.5);
   * @endcode
   * produces
   * @code
   *       10      20      30      40      50      60      70      80      90
   * +------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
   * ########################################
   * @endcode
   *
   * @attention Calling this function very frequently may gravely slow down your
   *  code! You should invoke this at most as often as the on-screen output
   *  changes.
   * @attention To overwrite the current line, the control character \c \\r is
   *  written to \c stream. When output is redirected to a file, this will cause
   *  strange looking output.
   * @attention To make your output align with the scale, call
   *  \c updateProgressBar(1.0) when your progress has finished.
   */
  void updateProgressBar(double percent, const int width = 80,
                         const char block = '#',
                         const ConsoleColorsTriple & format = {ConsoleColors::SPC_NORMAL},
                         std::ostream & stream = std::cout
  );

  //! @}
}

// ========================================================================== //
// template implementations

#include "BCG/Console.tpp"

#endif
