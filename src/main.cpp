// ========================================================================== //
// dependencies

// STL
#include <iostream>
// #include <iomanip>

#include <cstring>
#include <string>
using namespace std::string_literals;
#include <vector>

// own
#include "BCG.hpp"
#include "BCG_unittest.hpp"
#include "Parrot.hpp"

// ========================================================================== //
// unittest Parrot

std::string userPreparser   (const std::string & foo) {return foo;}
bool manualIntValidation    (int                 foo) {return foo == 42;}
bool manualStringValidation (const std::string & foo) {return foo == "bar";}

// -------------------------------------------------------------------------- //

void unittest_convenience() {
  BCG::writeBoxed("Testing the Definitions Convenience Functions", {BCG::ConsoleColors::FORE_YELLOW});

  std::cout
    << " \"foo bar\"   is of type: "
    << Parrot::valueTypeName( Parrot::valueTypeIDOf("foo bar") )
    << ", typeID: "
    << Parrot::getTypeIDOf("foo bar")
    << std::endl;

  std::cout
    << " \"foo bar\"s  is of type: "
    << Parrot::valueTypeName( Parrot::valueTypeIDOf("foo bar"s) )
    << ", typeID: "
    << Parrot::getTypeIDOf("foo bar"s)
    << std::endl;

  std::cout
    << "     1       is of type: "
    << Parrot::valueTypeName( Parrot::valueTypeIDOf(1) )
    << ", typeID: "
    << Parrot::getTypeIDOf(1)
    << std::endl;

  std::cout
    << "     1.0     is of type: "
    << Parrot::valueTypeName( Parrot::valueTypeIDOf(1.0) )
    << ", typeID: "
    << Parrot::getTypeIDOf(1.0)
    << std::endl;

  std::cout
    << "    true     is of type: "
    << Parrot::valueTypeName( Parrot::valueTypeIDOf(true) )
    << ", typeID: "
    << Parrot::getTypeIDOf(true)
    << std::endl;

  std::cout
    << "{\"foo bar\"}  is of type: "
    << Parrot::valueTypeName( Parrot::valueTypeIDOf({"foo bar"}) )
    << ", typeID: "
    << Parrot::getTypeIDOf({"foo bar"})
    << std::endl;

  std::cout
    << "{\"foo bar\"s} is of type: "
    << Parrot::valueTypeName( Parrot::valueTypeIDOf({"foo bar"s}) )
    << ", typeID: "
    << Parrot::getTypeIDOf({"foo bar"s})
    << std::endl;

  std::cout
    << "    {1}      is of type: "
    << Parrot::valueTypeName( Parrot::valueTypeIDOf({1}) )
    << ", typeID: "
    << Parrot::getTypeIDOf({1})
    << std::endl;

  std::cout
    << "    {1.0}    is of type: "
    << Parrot::valueTypeName( Parrot::valueTypeIDOf({1.0}) )
    << ", typeID: "
    << Parrot::getTypeIDOf({1.0})
    << std::endl;

  std::cout
    << "   {true}    is of type: "
    << Parrot::valueTypeName( Parrot::valueTypeIDOf({true}) )
    << ", typeID: "
    << Parrot::getTypeIDOf({true})
    << std::endl;


  std::cout << "retrieving the C++ data type from a Parrot::ValueTypeID:" << std::endl;
  Parrot::ValueType<Parrot::ValueTypeID::Real>::value_type x;
  if ( std::strcmp(typeid(x).name(), "d") ) { std::cout << "failed to retrieve data type" << std::endl; }
  else                                      { std::cout << "data type retrieved correctly" << std::endl; }

  std::cout << "rendering an std::any to text:" << std::endl;
  std::any anyPi = 3.141592654;
  std::cout << Parrot::getAnyText(anyPi) << std::endl << std::endl;

  std::cout << "(de)mangling constants:" << std::endl;
  std::cout << BCG::demangle(Parrot::TypeIDString_String.data()) << " alias ";
  std::cout << Parrot::TypeIDString_String << std::endl;

  std::cout << BCG::demangle(Parrot::TypeIDString_Integer.data()) << " alias ";
  std::cout << Parrot::TypeIDString_Integer << std::endl;

  std::cout << BCG::demangle(Parrot::TypeIDString_Real.data()) << " alias ";
  std::cout << Parrot::TypeIDString_Real << std::endl;

  std::cout << BCG::demangle(Parrot::TypeIDString_Boolean.data()) << " alias ";
  std::cout << Parrot::TypeIDString_Boolean << std::endl;

  std::cout << BCG::demangle(Parrot::TypeIDString_StringList.data()) << " alias ";
  std::cout << Parrot::TypeIDString_StringList << std::endl;

  std::cout << BCG::demangle(Parrot::TypeIDString_IntegerList.data()) << " alias ";
  std::cout << Parrot::TypeIDString_IntegerList << std::endl;

  std::cout << BCG::demangle(Parrot::TypeIDString_RealList.data()) << " alias ";
  std::cout << Parrot::TypeIDString_RealList << std::endl;

  std::cout << BCG::demangle(Parrot::TypeIDString_BooleanList.data()) << " alias ";
  std::cout << Parrot::TypeIDString_BooleanList << std::endl;
}
// .......................................................................... //
void unittest_Restriction() {
  BCG::writeBoxed("Testing the Restriction Class", {BCG::ConsoleColors::FORE_YELLOW});

  Parrot::Restriction rst;

  std::cout << "Default state of the Restriction class:" << std::endl;
  std::cout << rst.to_string() << std::endl << std::endl;

  std::cout << "setPreParseList and setRestrictionViolationText:" << std::endl;
  rst.setPreParseValidationList({"A", "B", "Cebra"});
  rst.setRestrictionViolationPolicy(Parrot::RestrictionViolationPolicy::Warning, "warning text");
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
  BCG::writeBoxed("Testing the Descriptor Class CTor and primitive setters", {BCG::ConsoleColors::FORE_YELLOW});

  Parrot::Descriptor  dsc;
  Parrot::Restriction rst;

  std::cout << "[0] Default state of the Descriptor class:" << std::endl;
  std::cout << dsc.to_string() << std::endl;

  std::cout << "[1] Adding a default value and multiple restrictions (explicitly):" << std::endl;
  dsc.setKey("foo bar");
  dsc.setValue(42);
  dsc.setValueCaseSensitive(true);
  dsc.addSubstitution("foo", "bar");
  dsc.addSubstitution("bar", "420");
  dsc.setUserPreParser( userPreparser );
  rst = Parrot::Restriction(-  1,   1, Parrot::RestrictionViolationPolicy::Exception, "QTY only defined on interval [-1:+1]");
  dsc.addRestriction(rst);
  rst = Parrot::Restriction(-.01, .01, Parrot::RestrictionViolationPolicy::Warning,   "QTY very close to zero");
  dsc.addRestriction(rst);
  std::cout << dsc.to_string() << std::endl;

  std::cout << "[2] reset:" << std::endl;
  dsc.reset();
  std::cout << "Type ID: " << dsc.getTypeID() << std::endl;
  std::cout << dsc.to_string() << std::endl;

  std::cout << "[3] CTor with explicit type:" << std::endl;
  dsc = Parrot::Descriptor("foo bar", Parrot::ValueTypeID::Integer);
  std::cout << "Type ID: " << dsc.getTypeID() << std::endl;
  std::cout << dsc.to_string() << std::endl;

  std::cout << "[4] CTor with implicit type:" << std::endl;
  dsc = Parrot::Descriptor("foo bar", "const char *");
  std::cout << "Type ID: " << dsc.getTypeID() << std::endl;
  std::cout << dsc.to_string() << std::endl;

  std::cout << "[5] CTor with implicit type:" << std::endl;
  dsc = Parrot::Descriptor("foo bar", "std::string"s);
  std::cout << "Type ID: " << dsc.getTypeID() << std::endl;
  std::cout << dsc.to_string() << std::endl;

  std::cout << "[6] CTor with implicit type:" << std::endl;
  dsc = Parrot::Descriptor("foo", std::vector<std::string>());
  std::cout << "Type ID: " << dsc.getTypeID() << std::endl;
  std::cout << dsc.to_string() << std::endl;

  std::cout << "[7] CTor with implicit type:" << std::endl;
  dsc = Parrot::Descriptor("foo", {"cstring list"});
  std::cout << "Type ID: " << dsc.getTypeID() << std::endl;
  std::cout << dsc.to_string() << std::endl;

  std::cout << "[8] CTor with implicit type:" << std::endl;
  dsc = Parrot::Descriptor("foo", {"c++ string list"s});
  std::cout << "Type ID: " << dsc.getTypeID() << std::endl;
  std::cout << dsc.to_string() << std::endl;

  std::cout << "[9] CTor with implicit type:" << std::endl;
  dsc = Parrot::Descriptor("foo", {true, false, true, true, true, false, false, false, true, true});
  std::cout << "Type ID: " << dsc.getTypeID() << std::endl;
  std::cout << dsc.to_string() << std::endl;
}
// .......................................................................... //
void unittest_Descriptor_make() {
  BCG::writeBoxed("Testing the Descriptor Class make* functions", {BCG::ConsoleColors::FORE_YELLOW});

  Parrot::Descriptor dsc;

  std::cout << "makeRanged, explicit:" << std::endl;
  dsc.makeRanged("foo bar", Parrot::ValueTypeID::IntegerList, -1, 1);
  std::cout << dsc.to_string() << std::endl;

  std::cout << "makeRanged, implicit:" << std::endl;
  dsc.makeRanged<std::vector<double>>("foo bar", {1., -1.}, -1, 1);
  std::cout << dsc.to_string() << std::endl;

  std::cout << "makeRanged, explicit, invalid type (boolean): " << std::flush;
  try {dsc.makeListboundAftParse("foo bar", Parrot::ValueTypeID::Boolean, std::vector<bool>({true, false}));}
  catch (std::exception & e) {std::cout << "prevented" << std::endl;}
  std::cout << std::endl;

  std::cout << "makeRanged, explicit, invalid type (string): " << std::flush;
  try {dsc.makeRanged("foo bar", Parrot::ValueTypeID::String, -1, 1);}
  catch (std::exception & e) {std::cout << "prevented" << std::endl;}
  std::cout << std::endl;



  std::cout << "makeListboundPreParse, explicit:" << std::endl;
  dsc.makeListboundPreParse("foo bar", Parrot::ValueTypeID::RealList, {"-1", "0", "1"});
  std::cout << dsc.to_string() << std::endl;

  std::cout << "makeListboundPreParse, implicit:" << std::endl;
  dsc.makeListboundPreParse("foo bar", std::vector<int>({1, 2, 3}), {"-1", "0", "1"}, true);
  std::cout << dsc.to_string() << std::endl;

  std::cout << "makeListboundPreParse, implicit, invalid type (boolean): " << std::flush;
  try {dsc.makeListboundPreParse("foo bar", std::vector<bool>({true, false}), {"active", "inactive"}, true);}
  catch (std::exception & e) {std::cout << "prevented" << std::endl;}
  std::cout << std::endl;



  std::cout << "makeListboundAftParse, explicit:" << std::endl;
  dsc.makeListboundAftParse("foo bar", Parrot::ValueTypeID::RealList, std::vector<double>({-1, 0, 1}));
  std::cout << "### complteded" << std::endl;
  std::cout << dsc.to_string() << std::endl;

  std::cout << "makeListboundAftParse, implicit: " << std::endl;
  dsc.makeListboundAftParse("foo bar", 10, std::vector<int>({-1, 0, 1}), true);
  std::cout << dsc.to_string() << std::endl;

  std::cout << "makeListboundAftParse, implicit:" << std::endl;
  dsc.makeListboundAftParse("foo bar", std::vector<double>({0.5}), std::vector<double>({-1, 0, 1}), true);
  std::cout << dsc.to_string() << std::endl;

  std::cout << "makeListboundAftParse, explicit, mismatched type (real list vs. int list): " << std::flush;
  try {dsc.makeListboundAftParse("foo bar", Parrot::ValueTypeID::RealList, std::vector<int>({-1, 0, 1}));}
  catch (std::exception & e) {std::cout << "prevented" << std::endl;}
  std::cout << std::endl;

  std::cout << "makeListboundAftParse, explicit, mismatched type (real vs. int list): " << std::flush;
  try {dsc.makeListboundAftParse("foo bar", Parrot::ValueTypeID::Real, std::vector<int>({-1, 0, 1}));}
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
  dsc.makeUserboundPreParse("foo bar", Parrot::ValueTypeID::BooleanList, manualStringValidation);
  std::cout << dsc.to_string() << std::endl;

  std::cout << "makeUserboundPreParse, implicit:" << std::endl;
  dsc.makeUserboundPreParse("foo bar", true, manualStringValidation);
  std::cout << dsc.to_string() << std::endl;



  std::cout << "makeUserboundAftParse, explicit:" << std::endl;
  dsc.makeUserboundAftParse<int>("foo bar", Parrot::ValueTypeID::Integer, manualIntValidation);
  std::cout << dsc.to_string() << std::endl;

  std::cout << "makeUserboundAftParse, implicit:" << std::endl;
  dsc.makeUserboundAftParse<int>("foo bar", 1, manualIntValidation);
  std::cout << dsc.to_string() << std::endl;

  std::cout << "makeUserboundAftParse, explicit, mismatched types (boolean list vs. string): " << std::flush;
  try {dsc.makeUserboundAftParse<std::string>("foo bar", Parrot::ValueTypeID::BooleanList, manualStringValidation);}
  catch (std::exception & e) {std::cout << "prevented" << std::endl;}
  std::cout << std::endl;

}

// ========================================================================== //
// main

int main () {
  BCG::init();
  BCG::writeBoxed("SETTINGS PACKAGE UNIT TEST", {BCG::ConsoleColors::FORE_GREEN}, 80, '=', '#', '#');

//   unittest_convenience();
//   unittest_Restriction();
  unittest_Descriptor_primitive();
//   unittest_Descriptor_make();

  std::cout << std::endl;
  BCG::writeBoxed("ALL DONE -- HAVE A NICE DAY!", {BCG::ConsoleColors::FORE_GREEN}, 80, '=', '#', '#');

}
