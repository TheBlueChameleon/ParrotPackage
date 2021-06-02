// ========================================================================== //
// dependencies

// STL
#include <stdexcept>

#include <iostream>

#include <vector>
#include <string>

#include <random>
#include <cmath>

// GMP
#include <gmp.h>
#include <gmpxx.h>

// own
#include "globals.hpp"

// ========================================================================== //
// generic

// -------------------------------------------------------------------------- //
// globals

// ========================================================================== //
// random number generator

std::random_device RNG;
std::uniform_real_distribution<> rand_phase_distribution;

double get_randPhase() {return rand_phase_distribution(RNG);}

// ========================================================================== //
// console output convenience

void consoleSetcolor (ConsoleColors code) {
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

void utterWarning (const std::string & text, 
                                 const std::string & headline,
                                 const int indentFirst,
                                 const int indentHanging,
                                 const ConsoleColorsTriple & headlineColors,
                                 const ConsoleColorsTriple &     textColors
) {
  
  if ( (indentFirst < 0) || (indentHanging < 0) ) {
    std::string errText = "Invalid argument in ";
    errText += __PRETTY_FUNCTION__;
    errText += ":\n";
    errText += "\tindent parameters needs to be greater than zero!";
    throw std::invalid_argument(errText);
  }
  
  
  consoleSetcolor(headlineColors.spc );
  consoleSetcolor(headlineColors.fore);
  consoleSetcolor(headlineColors.back);
  
  std::cout << std::string(indentFirst, ' ');
  std::cout << headline << std::endl;
  
  
  consoleSetcolor(textColors.spc );
  consoleSetcolor(textColors.fore);
  consoleSetcolor(textColors.back);
  
  int indentTotal = indentFirst + indentHanging;
  const std::string indent(indentTotal, ' ');
  
  auto lines = splitString(text, '\n');
  for (auto & line : lines) {
    std::cout << indent;
    std::cout << line;
    std::cout << std::endl;
  }
}
// .......................................................................... //
void coutHeadline ( const std::string & text,
                    const ConsoleColorsTriple & headlineColors
) {
  
  auto lines = splitString(text, '\n');
  
  consoleSetcolor(headlineColors.spc );
  consoleSetcolor(headlineColors.fore);
  consoleSetcolor(headlineColors.back);
  
  std::cout << "+-----------------------------------------------------------------------------+" << std::endl;
  for (auto & line : lines) {
    std::cout << "| ";
    std::cout << line;
    std::cout << std::string(75 - line.size(), ' ');
    std::cout << " |\n";
  }
  std::cout << "+-----------------------------------------------------------------------------+" << std::endl;
  
  consoleSetcolor(ConsoleColors::SPC_NORMAL);
}
// ========================================================================== //
// String utility

std::vector<std::string> splitString(const std::string & s, const char separator) {
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
bool wildcardmatch(const char *first, const char * second) {
  // https://www.geeksforgeeks.org/wildcard-character-matching/
  
  if (*first == '\0' && *second == '\0')                        { return true ; }
  if (*first == '*'  && *(first+1) != '\0' && *second == '\0')  { return false; }
  if (*first == '?'  || *first == *second)                      { return wildcardmatch(first+1, second+1); }
  if (*first == '*')                                            { return wildcardmatch(first+1, second) || wildcardmatch(first, second+1); }
  return false; 
}
bool wildcardmatch(const std::string & pattern, const std::string & searchstring) {return wildcardmatch(pattern.c_str(), searchstring.c_str());}

// ========================================================================== //
// File utility

// .......................................................................... //
std::string  generateTimestamp() {
  std::string reVal;
  
  // adapted from https://stackoverflow.com/questions/16357999/current-date-and-time-as-string
  time_t      rawtime  = time     ( nullptr);
  struct tm * timeinfo = localtime(&rawtime);
  
  char buffer[80];
  strftime(buffer, sizeof(buffer), "%Y-%m-%d, %H:%M:%S", timeinfo);
  
  reVal = buffer;
  
  return reVal;
}

// ========================================================================== //
// misc

std::vector<double> linspace(const double start, const double end, const int N) {
  std::string errText;
  
  if (N < 0) {
    errText  = "Invalid argument in linspace:\n";
    errText += "\tparameter 'N' needs to be greater than zero!";
    throw std::invalid_argument(errText);
  }
  
  if ( std::isnan(start) || std::isnan(end) ) {
    errText  = "Invalid argument in linspace:\n";
    errText += "\tEither 'start' or 'end' is NAN!";
    throw std::invalid_argument(errText);
  }
  
  std::vector<double> reVal(N);
  
  if (N == 1) {reVal[0] = start; return reVal;}
  
  auto delta = (end - start) / (N-1);
  auto alpha = start;
  
  for (auto i=0; i<N; ++i) {
    reVal[i] = alpha;
    alpha += delta;
  }
  
  return reVal;
}

