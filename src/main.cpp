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

std::string userPreparser   (const std::string & foo) {return foo;}
bool manualStringValidation (const std::string & foo) {return foo == "bar";}
bool manualIntValidation    (int                 foo) {return foo == 42;}

// -------------------------------------------------------------------------- //

// ========================================================================== //
// main

int main () {
  consoleSetcolor(ConsoleColors::FORE_GREEN);
  std::cout << "# ============================================================================ #" << std::endl;
  std::cout << "# SETTINGS PACKAGE UNIT TEST                                                   #" << std::endl;
  std::cout << "# ============================================================================ #" << std::endl;
  std::cout << std::endl;

  coutHeadline("Testing the Definitions Convenience Functions", {ConsoleColors::FORE_YELLOW});
  {
    std::cout << Settings::valueTypeName( Settings::valueTypeOf("bar"      ) ) << std::endl;
    std::cout << Settings::valueTypeName( Settings::valueTypeOf({1}        ) ) << std::endl;
    std::cout << Settings::valueTypeName( Settings::valueTypeOf({"foo bar"}) ) << std::endl;
  }

  coutHeadline("Testing the Restriction Class", {ConsoleColors::FORE_YELLOW});
  {
    Settings::Restriction rst;
    
    std::cout << "Default state of the Restriction class:" << std::endl;
    std::cout << rst.to_string() << std::endl << std::endl;
    
    std::cout << "setPreParseList and setRestrictionViolationText:" << std::endl;
    rst.setPreParseValidationList({"A", "B", "Cebra"});
    rst.setRestrictionViolationPolicy(Settings::RestrictionViolationPolicy::Warning, "warning text");
    std::cout << rst.to_string()  << std::endl;
    
    std::cout << "setAftParseList:" << std::endl;
    rst.setAftParseValidationList<std::string>({"C", "b", "Aebra"}, true);
    std::cout << rst.to_string()  << std::endl;
    
    std::cout << "reset:" << std::endl;
    rst.reset();
    std::cout << rst.to_string()  << std::endl;
    
    std::cout << "setPreParseFunction and setAftParseFunction:" << std::endl;
    rst.setPreParseValidationFunction(manualStringValidation);
    rst.setAftParseValidationFunction<int>(manualIntValidation);
    std::cout << rst.to_string()  << std::endl;
  }
  
  coutHeadline("Testing the Descriptor Class", {ConsoleColors::FORE_YELLOW});
  {
    Settings::Descriptor  dsc;
    Settings::Restriction rst;

    std::cout << "Default state of the Descriptor class:" << std::endl;
    std::cout << dsc.to_string() << std::endl;

    std::cout << "Adding a default value and multiple restrictions (explicitly):" << std::endl;
    dsc.setKey("foo bar");
    dsc.setValue(1);
    dsc.setValueCaseSensitive(true);
    dsc.addSubstitution("foo", "bar");
    dsc.addSubstitution("bar", "420");
    dsc.setUserPreParser(userPreparser);
    rst = Settings::Restriction(-  1,   1, Settings::RestrictionViolationPolicy::Exception, "QTY only defined on interval [-1:+1]");
    dsc.addRestriction(rst);
    rst = Settings::Restriction(-.01, .01, Settings::RestrictionViolationPolicy::Warning,   "QTY very close to zero");
    dsc.addRestriction(rst);
    std::cout << dsc.to_string() << std::endl;

    std::cout << "reset:" << std::endl;
    dsc.reset();
    std::cout << dsc.getTypeID() << std::endl;
    std::cout << dsc.to_string() << std::endl;

    std::cout << "CTor with explicit type:" << std::endl;
    dsc = Settings::Descriptor("foo bar", Settings::ValueType::Integer);
    std::cout << dsc.getTypeID() << std::endl;
    std::cout << dsc.to_string() << std::endl;

    std::cout << "CTor with implicit type:" << std::endl;
    dsc = Settings::Descriptor("foo bar", "const char *");
    std::cout << dsc.getTypeID() << std::endl;
    std::cout << dsc.to_string() << std::endl;

    std::cout << "CTor with implicit type:" << std::endl;
    dsc = Settings::Descriptor("foo bar", "std::string"s);
    std::cout << dsc.getTypeID() << std::endl;
    std::cout << dsc.to_string() << std::endl;

    std::cout << "CTor with implicit type:" << std::endl;
    dsc = Settings::Descriptor("foo", std::vector<std::string>());
    std::cout << dsc.getTypeID() << std::endl;
    std::cout << dsc.to_string() << std::endl;

    std::cout << "CTor with implicit type:" << std::endl;
    dsc = Settings::Descriptor("foo", {"string list"});
    std::cout << dsc.getTypeID() << std::endl;
    std::cout << dsc.to_string() << std::endl;

    std::cout << "CTor with implicit type:" << std::endl;
    dsc = Settings::Descriptor("foo", {true, false, true});
    std::cout << dsc.getTypeID() << std::endl;
    std::cout << dsc.to_string() << std::endl;

    std::cout << "makeRanged, explicit:" << std::endl;
    dsc.makeRanged("foo bar", Settings::ValueType::IntegerList, -1, 1);
    std::cout << dsc.to_string() << std::endl;

    std::cout << "makeRanged, implicit:" << std::endl;
    dsc.makeRanged<std::vector<double>>("foo bar", {1., -1.}, -1, 1);
    std::cout << dsc.to_string() << std::endl;
  }

}
