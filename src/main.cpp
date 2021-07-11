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

std::string userPreparser   (const PARROT_TYPE(Parrot::ValueTypeID::String ) & foo) {return foo;}
bool manualIntValidation    (const PARROT_TYPE(Parrot::ValueTypeID::Integer) & foo) {return foo ==    42;}
bool manualStringValidation (const PARROT_TYPE(Parrot::ValueTypeID::String ) & foo) {return foo == "bar";}

// -------------------------------------------------------------------------- //

void unittest_convenience() {
  BCG::writeBoxed("Testing the Definitions Convenience Functions", {BCG::ConsoleColors::FORE_YELLOW});

  std::cout
    << " \"foo bar\"   is of type: "
    << Parrot::valueTypeName( Parrot::valueTypeIDOf("foo bar") )
    << std::endl;

  std::cout
    << " \"foo bar\"s  is of type: "
    << Parrot::valueTypeName( Parrot::valueTypeIDOf("foo bar"s) )
    << std::endl;

  std::cout
    << "     1       is of type: "
    << Parrot::valueTypeName( Parrot::valueTypeIDOf(1) )
    << std::endl;

  std::cout
    << "     1.0     is of type: "
    << Parrot::valueTypeName( Parrot::valueTypeIDOf(1.0) )
    << std::endl;

  std::cout
    << "    true     is of type: "
    << Parrot::valueTypeName( Parrot::valueTypeIDOf(true) )
    << std::endl;

  std::cout
    << "{\"foo bar\"}  is of type: "
    << Parrot::valueTypeName( Parrot::valueTypeIDOf({"foo bar"}) )
    << std::endl;

  std::cout
    << "{\"foo bar\"s} is of type: "
    << Parrot::valueTypeName( Parrot::valueTypeIDOf({"foo bar"s}) )
    << std::endl;

  std::cout
    << "    {1}      is of type: "
    << Parrot::valueTypeName( Parrot::valueTypeIDOf({1}) )
    << std::endl;

  std::cout
    << "    {1.0}    is of type: "
    << Parrot::valueTypeName( Parrot::valueTypeIDOf({1.0}) )
    << std::endl;

  std::cout
    << "   {true}    is of type: "
    << Parrot::valueTypeName( Parrot::valueTypeIDOf({true}) )
    << std::endl;
  std::cout << std::endl;


  // quick'n'dirty test -- may fail on compilers other than gcc and clang
  std::cout << "retrieving the C++ data type from a Parrot::ValueTypeID: " << std::flush;
  Parrot::ValueType<Parrot::ValueTypeID::Real>::value_type x;
  if ( std::strcmp(typeid(x).name(), "d") ) { std::cout << "failed to retrieve data type" << std::endl; }
  else                                      { std::cout << "data type retrieved correctly" << std::endl; }
  std::cout << std::endl;


  std::cout << "rendering an std::any to text: " << std::flush;
  std::any anyPi = 3.141592654;
  std::cout << Parrot::getAnyText(anyPi) << std::endl;
  std::cout << std::endl;


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

  std::cout << "Manual String  Validation Function at " << (void *) manualStringValidation << std::endl;
  std::cout << "Manual Integer Validation Function at " << (void *) manualIntValidation << std::endl;
  std::cout << std::endl;

  Parrot::Restriction rst;

  std::cout << "[0] Default state of the Restriction class:" << std::endl;
  std::cout << rst.to_string() << std::endl;

  std::cout << "[1] setPreParseList and setRestrictionViolationText:" << std::endl;
  rst.setPreParseValidationList({"A", "B", "Cebra"});
  rst.setRestrictionViolationPolicy(Parrot::RestrictionViolationPolicy::Warning, "warning text");
  std::cout << rst.to_string()  << std::endl;

  std::cout << "[2] setAftParseList:" << std::endl;
  rst.setAftParseValidationList<std::string>({"C", "b", "Aebra"}, true);
  std::cout << rst.to_string()  << std::endl;

  std::cout << "[3] reset:" << std::endl;
  rst.reset();
  std::cout << rst.to_string()  << std::endl;

  std::cout << "[4] setPreParseFunction and setAftParseFunction:" << std::endl;
  rst.setPreParseValidationFunction              (manualStringValidation);
  rst.setAftParseValidationFunction(std::function(manualIntValidation));
  std::cout << rst.to_string()  << std::endl;
}
// .......................................................................... //
void unittest_Descriptor_primitive() {
  BCG::writeBoxed("Testing the Descriptor Class CTor and primitive setters", {BCG::ConsoleColors::FORE_YELLOW});

  std::cout << "User Parsing Function at " << (void *) userPreparser << std::endl;
  std::cout << std::endl;

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

  std::cout << "[0] makeRanged, explicit:" << std::endl;
  dsc.makeRanged("foo bar", Parrot::ValueTypeID::IntegerList, -1337, 1337);
  std::cout << dsc.to_string() << std::endl;

  std::cout << "[1] makeRanged, implicit:" << std::endl;
  dsc.makeRanged<std::vector<double>>("foo bar", {3.141592654, -3.141592654}, -3.141592654, 3.141592654);
  std::cout << dsc.to_string() << std::endl;

  std::cout << "[2] makeRanged, explicit, invalid type (boolean): " << std::flush;
  try {dsc.makeListboundAftParse("foo bar", Parrot::ValueTypeID::Boolean, std::vector<bool>({true, false}));}
  catch (std::exception & e) {std::cout << "prevented" << std::endl;}
  std::cout << std::endl;

  std::cout << "[3] makeRanged, explicit, invalid type (string): " << std::flush;
  try {dsc.makeRanged("foo bar", Parrot::ValueTypeID::String, -1, 1);}
  catch (std::exception & e) {std::cout << "prevented" << std::endl;}
  std::cout << std::endl;



  std::cout << "[4] makeListboundPreParse, explicit:" << std::endl;
  dsc.makeListboundPreParse("foo bar", Parrot::ValueTypeID::RealList, {"-1", "0", "1"});
  std::cout << dsc.to_string() << std::endl;

  std::cout << "[5] makeListboundPreParse, implicit:" << std::endl;
  dsc.makeListboundPreParse("foo bar", std::vector<int>({1, 2, 3}), {"-1", "0", "1"}, true);
  std::cout << dsc.to_string() << std::endl;

  std::cout << "[6] makeListboundPreParse, implicit, invalid type (boolean): " << std::flush;
  try {dsc.makeListboundPreParse("foo bar", std::vector<bool>({true, false}), {"active", "inactive"}, true);}
  catch (std::exception & e) {std::cout << "prevented" << std::endl;}
  std::cout << std::endl;



  std::cout << "[7] makeListboundAftParse, explicit:" << std::endl;
  dsc.makeListboundAftParse("foo bar", Parrot::ValueTypeID::RealList, std::vector<double>({-1, 0, 1}));
  std::cout << dsc.to_string() << std::endl;

  std::cout << "[8] makeListboundAftParse, implicit: " << std::endl;
  dsc.makeListboundAftParse("foo bar", 1337, std::vector<int>({-1, 0, 1}), true);
  std::cout << dsc.to_string() << std::endl;

  std::cout << "[9] makeListboundAftParse, implicit:" << std::endl;
  dsc.makeListboundAftParse("foo bar", std::vector<double>({3.141592654}), std::vector<double>({-1, 0, 1}), true);
  std::cout << dsc.to_string() << std::endl;

  std::cout << "[10] makeListboundAftParse, explicit, mismatched type (real list vs. int list): " << std::flush;
  try {dsc.makeListboundAftParse("foo bar", Parrot::ValueTypeID::RealList, std::vector<int>({-1, 0, 1}));}
  catch (std::exception & e) {std::cout << "prevented" << std::endl;}
  std::cout << std::endl;

  std::cout << "[11] makeListboundAftParse, explicit, mismatched type (real vs. int list): " << std::flush;
  try {dsc.makeListboundAftParse("foo bar", Parrot::ValueTypeID::Real, std::vector<int>({-1, 0, 1}));}
  catch (std::exception & e) {std::cout << "prevented" << std::endl;}
  std::cout << std::endl;

  std::cout << "[12] makeListboundAftParse, implicit, mismatched type (real list vs. int list): " << std::flush;
  try {dsc.makeListboundAftParse("foo bar", std::vector<double>({0.5}), std::vector<int>({-1, 0, 1}), true);}
  catch (std::exception & e) {std::cout << "prevented" << std::endl;}
  std::cout << std::endl;

  std::cout << "[13] makeListboundAftParse, implicit, mismatched type (real vs. int list): " << std::flush;
  try {dsc.makeListboundAftParse("foo bar", 0.5, std::vector<int>({-1, 0, 1}), true);}
  catch (std::exception & e) {std::cout << "prevented" << std::endl;}
  std::cout << std::endl;


  std::cout << "[14] makeUserboundPreParse, explicit:" << std::endl;
  dsc.makeUserboundPreParse("foo bar", Parrot::ValueTypeID::BooleanList, manualStringValidation);
  std::cout << dsc.to_string() << std::endl;

  std::cout << "[15] makeUserboundPreParse, implicit:" << std::endl;
  dsc.makeUserboundPreParse("foo bar", true, manualStringValidation);
  std::cout << dsc.to_string() << std::endl;



  std::cout << "[16] makeUserboundAftParse, explicit:" << std::endl;
  dsc.makeUserboundAftParse("foo bar", Parrot::ValueTypeID::Integer, std::function(manualIntValidation));
  std::cout << dsc.to_string() << std::endl;

  std::cout << "[17] makeUserboundAftParse, implicit:" << std::endl;
  dsc.makeUserboundAftParse("foo bar", 1ll, std::function(manualIntValidation));
  std::cout << dsc.to_string() << std::endl;

  std::cout << "[18] makeUserboundAftParse, explicit, mismatched types (boolean list vs. string): " << std::flush;
  try {dsc.makeUserboundAftParse<std::string>("foo bar", Parrot::ValueTypeID::BooleanList, manualStringValidation);}
  catch (std::exception & e) {std::cout << "prevented" << std::endl;}
  std::cout << std::endl;

}

// ========================================================================== //
// main

int main () {
  BCG::init();
  BCG::writeBoxed("SETTINGS PACKAGE UNIT TEST", {BCG::ConsoleColors::FORE_GREEN}, 80, '=', '#', '#');

//   std::cout << BCG::getTypeName( std::vector({"asdf", "jklö"}) ) << std::endl;
//   return 0;

//   unittest_convenience();
  unittest_Restriction();
  unittest_Descriptor_primitive();
//   unittest_Descriptor_make();

  std::cout << std::endl;
  BCG::writeBoxed("ALL DONE -- HAVE A NICE DAY!", {BCG::ConsoleColors::FORE_GREEN}, 80, '=', '#', '#');

}
