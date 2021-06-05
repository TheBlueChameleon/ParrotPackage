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
bool manualIntValidation    (int                 foo) {return foo == 42;}
bool manualStringValidation (const std::string & foo) {return foo == "bar";}

// -------------------------------------------------------------------------- //

void unittest_convenience() {
  coutHeadline("Testing the Definitions Convenience Functions", {ConsoleColors::FORE_YELLOW});

  std::cout
    << " \"foo bar\"   is of type: "
    << Settings::valueTypeName( Settings::valueTypeOf("foo bar") )
    << ", typeID: "
    << Settings::getTypeIDOf("foo bar")
    << std::endl;

  std::cout
    << " \"foo bar\"s  is of type: "
    << Settings::valueTypeName( Settings::valueTypeOf("foo bar"s) )
    << ", typeID: "
    << Settings::getTypeIDOf("foo bar"s)
    << std::endl;

  std::cout
    << "     1       is of type: "
    << Settings::valueTypeName( Settings::valueTypeOf(1) )
    << ", typeID: "
    << Settings::getTypeIDOf(1)
    << std::endl;

  std::cout
    << "     1.0     is of type: "
    << Settings::valueTypeName( Settings::valueTypeOf(1.0) )
    << ", typeID: "
    << Settings::getTypeIDOf(1.0)
    << std::endl;

  std::cout
    << "    true     is of type: "
    << Settings::valueTypeName( Settings::valueTypeOf(true) )
    << ", typeID: "
    << Settings::getTypeIDOf(true)
    << std::endl;

  std::cout
    << "{\"foo bar\"}  is of type: "
    << Settings::valueTypeName( Settings::valueTypeOf({"foo bar"}) )
    << ", typeID: "
    << Settings::getTypeIDOf({"foo bar"})
    << std::endl;

  std::cout
    << "{\"foo bar\"s} is of type: "
    << Settings::valueTypeName( Settings::valueTypeOf({"foo bar"s}) )
    << ", typeID: "
    << Settings::getTypeIDOf({"foo bar"s})
    << std::endl;

  std::cout
    << "    {1}      is of type: "
    << Settings::valueTypeName( Settings::valueTypeOf({1}) )
    << ", typeID: "
    << Settings::getTypeIDOf({1})
    << std::endl;

  std::cout
    << "    {1.0}    is of type: "
    << Settings::valueTypeName( Settings::valueTypeOf({1.0}) )
    << ", typeID: "
    << Settings::getTypeIDOf({1.0})
    << std::endl;

  std::cout
    << "   {true}    is of type: "
    << Settings::valueTypeName( Settings::valueTypeOf({true}) )
    << ", typeID: "
    << Settings::getTypeIDOf({true})
    << std::endl;
}
// .......................................................................... //
void unittest_Restriction() {
  coutHeadline("Testing the Restriction Class", {ConsoleColors::FORE_YELLOW});

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
// .......................................................................... //
void unittest_Descriptor_primitive() {
  coutHeadline("Testing the Descriptor Class CTor and primitive setters", {ConsoleColors::FORE_YELLOW});

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
}
// .......................................................................... //
void unittest_Descriptor_make() {
  coutHeadline("Testing the Descriptor Class make* functions", {ConsoleColors::FORE_YELLOW});

  Settings::Descriptor dsc;

  std::cout << "makeRanged, explicit:" << std::endl;
  dsc.makeRanged("foo bar", Settings::ValueType::IntegerList, -1, 1);
  std::cout << dsc.to_string() << std::endl;

  std::cout << "makeRanged, implicit:" << std::endl;
  dsc.makeRanged<std::vector<double>>("foo bar", {1., -1.}, -1, 1);
  std::cout << dsc.to_string() << std::endl;

  std::cout << "makeRanged, explicit, invalid type (boolean): " << std::flush;
  try {dsc.makeListboundAftParse("foo bar", Settings::ValueType::Boolean, std::vector<bool>({true, false}));}
  catch (std::exception & e) {std::cout << "prevented" << std::endl;}
  std::cout << std::endl;

  std::cout << "makeRanged, explicit, invalid type (string): " << std::flush;
  try {dsc.makeRanged("foo bar", Settings::ValueType::String, -1, 1);}
  catch (std::exception & e) {std::cout << "prevented" << std::endl;}
  std::cout << std::endl;



  std::cout << "makeListboundPreParse, explicit:" << std::endl;
  dsc.makeListboundPreParse("foo bar", Settings::ValueType::RealList, {"-1", "0", "1"});
  std::cout << dsc.to_string() << std::endl;

  std::cout << "makeListboundPreParse, implicit:" << std::endl;
  dsc.makeListboundPreParse("foo bar", std::vector<int>({1, 2, 3}), {"-1", "0", "1"}, true);
  std::cout << dsc.to_string() << std::endl;

  std::cout << "makeListboundPreParse, implicit, invalid type (boolean): " << std::flush;
  try {dsc.makeListboundPreParse("foo bar", std::vector<bool>({true, false}), {"active", "inactive"}, true);}
  catch (std::exception & e) {std::cout << "prevented" << std::endl;}
  std::cout << std::endl;



  std::cout << "makeListboundAftParse, explicit:" << std::endl;
  dsc.makeListboundAftParse("foo bar", Settings::ValueType::RealList, std::vector<double>({-1, 0, 1}));
  std::cout << dsc.to_string() << std::endl;

  std::cout << "makeListboundAftParse, implicit: " << std::endl;
  dsc.makeListboundAftParse("foo bar", 10, std::vector<int>({-1, 0, 1}), true);
  std::cout << dsc.to_string() << std::endl;

  std::cout << "makeListboundAftParse, implicit:" << std::endl;
  dsc.makeListboundAftParse("foo bar", std::vector<double>({0.5}), std::vector<double>({-1, 0, 1}), true);
  std::cout << dsc.to_string() << std::endl;

  std::cout << "makeListboundAftParse, explicit, mismatched type (real list vs. int list): " << std::flush;
  try {dsc.makeListboundAftParse("foo bar", Settings::ValueType::RealList, std::vector<int>({-1, 0, 1}));}
  catch (std::exception & e) {std::cout << "prevented" << std::endl;}
  std::cout << std::endl;

  std::cout << "makeListboundAftParse, explicit, mismatched type (real vs. int list): " << std::flush;
  try {dsc.makeListboundAftParse("foo bar", Settings::ValueType::Real, std::vector<int>({-1, 0, 1}));}
  catch (std::exception & e) {std::cout << "prevented" << std::endl;}
  std::cout << std::endl;

  std::cout << "makeListboundAftParse, implicit, mismatched type (real list vs. int list): " << std::flush;
  try {dsc.makeListboundAftParse("foo bar", std::vector<double>({0.5}), std::vector<int>({-1, 0, 1}), true);}
  catch (std::exception & e) {std::cout << "prevented" << std::endl;}
  std::cout << std::endl;

  std::cout << "makeListboundAftParse, implicit, mismatched type (real vs. int list): " << std::flush;
  try {dsc.makeListboundAftParse("foo bar", 0.5, std::vector<int>({-1, 0, 1}), true);}
  catch (std::exception & e) {std::cout << "prevented" << std::endl;}
  std::cout << std::endl;


  std::cout << "makeUserboundPreParse, explicit:" << std::endl;
  dsc.makeUserboundPreParse("foo bar", Settings::ValueType::BooleanList, manualStringValidation);
  std::cout << dsc.to_string() << std::endl;

  std::cout << "makeUserboundPreParse, implicit:" << std::endl;
  dsc.makeUserboundPreParse("foo bar", true, manualStringValidation);
  std::cout << dsc.to_string() << std::endl;



  std::cout << "makeUserboundAftParse, explicit:" << std::endl;
  dsc.makeUserboundAftParse<int>("foo bar", Settings::ValueType::Integer, manualIntValidation);
  std::cout << dsc.to_string() << std::endl;

  std::cout << "makeUserboundAftParse, implicit:" << std::endl;
  dsc.makeUserboundAftParse<int>("foo bar", 1, manualIntValidation);
  std::cout << dsc.to_string() << std::endl;

  std::cout << "makeUserboundAftParse, explicit, mismatched types (boolean list vs. string): " << std::flush;
  try {dsc.makeUserboundAftParse<std::string>("foo bar", Settings::ValueType::BooleanList, manualStringValidation);}
  catch (std::exception & e) {std::cout << "prevented" << std::endl;}
  std::cout << std::endl;

}
// ========================================================================== //
// main

int main () {
  consoleSetcolor(ConsoleColors::FORE_GREEN);
  std::cout << "# ============================================================================ #" << std::endl;
  std::cout << "# SETTINGS PACKAGE UNIT TEST                                                   #" << std::endl;
  std::cout << "# ============================================================================ #" << std::endl;
  std::cout << std::endl;

//   unittest_convenience();
//   unittest_Restriction();
//   unittest_Descriptor_primitive();
//   unittest_Descriptor_make();
}
