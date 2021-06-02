// ========================================================================== //
// dependencies

// STL
#include <iostream>

#include <string>
using namespace std::string_literals;
#include <any>

// own
#include "Settings.hpp"

// ========================================================================== //
// proc

bool noArgs() {return true;}
bool oneArg(int foo) {return foo == 42;}

// ========================================================================== //
// main

int main () {
  std::cout << "hello world" << std::endl;
  
  Settings::Descriptor dsc, dvoid;
  
  dsc.key = "the world";
  dsc.value = "an oyster"s;
  
  std::cout << std::any_cast<std::string>(dsc.value)<< std::endl;
  std::cout << dvoid.value.has_value() << std::endl;
  
  dsc.value.reset();
  std::cout << dsc.value.has_value() << std::endl;
  
  Settings::Restriction rst;
}
