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
  
  coutHeadline("Testing the Restriction Class", {ConsoleColors::FORE_WHITE});
  {
    
    Settings::Restriction rst;
    
    std::cout << "Default state of the Restriction class:" << std::endl;
    std::cout << rst.to_string() << std::endl << std::endl;
    
    std::cout << "setPreParseList and setRestrictionViolationText:" << std::endl;
    rst.setPreParseList({"A", "B", "Cebra"});
    rst.setRestrictionViolationPolicy(Settings::RestrictionViolationPolicy::Warning, "warning text");
    std::cout << rst.to_string()  << std::endl;
    
    std::cout << "setAftParseList:" << std::endl;
    rst.setAftParseList<std::string>({"C", "b", "Aebra"}, true);
    std::cout << rst.to_string()  << std::endl;
    
    std::cout << "reset:" << std::endl;
    rst.reset();
    std::cout << rst.to_string()  << std::endl;
    
    std::cout << "setPreParseFunction and setAftParseFunction:" << std::endl;
    rst.setPreParseFunction(manualStringValidation);
    rst.setAftParseFunction<int>(manualIntValidation);
    std::cout << rst.to_string()  << std::endl;
    
  }
  
  coutHeadline("Testing the Descriptor Class", {ConsoleColors::FORE_WHITE});
  {
    Settings::Descriptor dsc;
    
//     dsc.setValue(1);
//     dsc.setValue(1.);
//     dsc.setValue(true);
//     dsc.setValue("foo bar");
    dsc.setValue<int>         ({1});
//     dsc.setValue<double>      ({1.});
//     dsc.setValue<bool>        ({true});
//     dsc.setValue<std::string> ({"foo bar"});
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
