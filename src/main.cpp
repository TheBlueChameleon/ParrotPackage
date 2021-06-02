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

bool manualStringValidation(std::string foo) {return foo == "bar";}
bool manualIntValidation   (int         foo) {return foo == 42;}

// -------------------------------------------------------------------------- //

// ========================================================================== //
// main

int main () {
  consoleSetcolor(ConsoleColors::FORE_GREEN);
  std::cout << "# ============================================================================ #" << std::endl;
  std::cout << "# SETTINGS PACKAGE UNIT TEST                                                   #" << std::endl;
  std::cout << "# ============================================================================ #" << std::endl;
  std::cout << std::endl;
  
  {
    coutHeadline("Testing the Restriction Class", {ConsoleColors::FORE_WHITE});
    
    std::cout << "Default state of the Restriction class:" << std::endl;
    Settings::Restriction rst_empty;
    std::cout << rst_empty.to_string() << std::endl << std::endl;
    
    std::cout << "Modified Restriction class instances:" << std::endl;
    Settings::Restriction rst_mod;
    
    rst_mod.setPreParseList({"A", "B", "Cebra"});
    std::cout << rst_mod.to_string()  << std::endl;
    
    rst_mod.setPreParseList({"A", "B", "Cebra"}, true);
    std::cout << rst_mod.to_string()  << std::endl;
    
    rst_mod.reset();
    std::cout << rst_mod.to_string()  << std::endl;
    
    rst_mod.setPreParseRange(0,1);
    std::cout << rst_mod.to_string()  << std::endl;
    
    rst_mod.setPreParseFunction(manualStringValidation);
    rst_mod.setAftParseFunction<int>(manualIntValidation);
    rst_mod.setRestrictionViolationText("warning text", false);
    std::cout << rst_mod.to_string()  << std::endl;
    
  }
  
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
  
}
