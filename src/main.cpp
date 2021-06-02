// ========================================================================== //
// dependencies

// STL
#include <iostream>

#include <string>
using namespace std::string_literals;
#include <any>

// own
#include "globals.hpp"
#include "Settings.hpp"

// ========================================================================== //
// proc

bool oneArg(int foo) {return foo == 42;}

// -------------------------------------------------------------------------- //

// ========================================================================== //
// main

int main () {
  coutHeadline("SETTINGS PACKAGE UNIT TEST", {ConsoleColors::FORE_GREEN});
  std::cout << std::endl;
  
//   Settings::Descriptor dsc, dvoid;
//   
//   dsc.key = "the world";
//   dsc.value = "an oyster"s;
//   
//   std::cout << std::any_cast<std::string>(dsc.value)<< std::endl;
//   std::cout << dvoid.value.has_value() << std::endl;
//   
//   dsc.value.reset();
//   std::cout << dsc.value.has_value() << std::endl;
  
  Settings::Restriction rst;
  std::cout << rst.to_string()  << std::endl;
}
