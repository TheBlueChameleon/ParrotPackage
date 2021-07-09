// ========================================================================== //
// dependencies

// STL
#include <iostream>
// #include <iomanip>
//
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
    << Parrot::valueTypeName( Parrot::valueTypeOf("foo bar") )
    << ", typeID: "
    << Parrot::getTypeIDOf("foo bar")
    << std::endl;

  std::cout
    << " \"foo bar\"s  is of type: "
    << Parrot::valueTypeName( Parrot::valueTypeOf("foo bar"s) )
    << ", typeID: "
    << Parrot::getTypeIDOf("foo bar"s)
    << std::endl;

  std::cout
    << "     1       is of type: "
    << Parrot::valueTypeName( Parrot::valueTypeOf(1) )
    << ", typeID: "
    << Parrot::getTypeIDOf(1)
    << std::endl;

  std::cout
    << "     1.0     is of type: "
    << Parrot::valueTypeName( Parrot::valueTypeOf(1.0) )
    << ", typeID: "
    << Parrot::getTypeIDOf(1.0)
    << std::endl;

  std::cout
    << "    true     is of type: "
    << Parrot::valueTypeName( Parrot::valueTypeOf(true) )
    << ", typeID: "
    << Parrot::getTypeIDOf(true)
    << std::endl;

  std::cout
    << "{\"foo bar\"}  is of type: "
    << Parrot::valueTypeName( Parrot::valueTypeOf({"foo bar"}) )
    << ", typeID: "
    << Parrot::getTypeIDOf({"foo bar"})
    << std::endl;

  std::cout
    << "{\"foo bar\"s} is of type: "
    << Parrot::valueTypeName( Parrot::valueTypeOf({"foo bar"s}) )
    << ", typeID: "
    << Parrot::getTypeIDOf({"foo bar"s})
    << std::endl;

  std::cout
    << "    {1}      is of type: "
    << Parrot::valueTypeName( Parrot::valueTypeOf({1}) )
    << ", typeID: "
    << Parrot::getTypeIDOf({1})
    << std::endl;

  std::cout
    << "    {1.0}    is of type: "
    << Parrot::valueTypeName( Parrot::valueTypeOf({1.0}) )
    << ", typeID: "
    << Parrot::getTypeIDOf({1.0})
    << std::endl;

  std::cout
    << "   {true}    is of type: "
    << Parrot::valueTypeName( Parrot::valueTypeOf({true}) )
    << ", typeID: "
    << Parrot::getTypeIDOf({true})
    << std::endl;
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

  std::cout << "Default state of the Descriptor class:" << std::endl;
  std::cout << dsc.to_string() << std::endl;

  std::cout << "Adding a default value and multiple restrictions (explicitly):" << std::endl;
  dsc.setKey("foo bar");
  dsc.setValue(1);
  dsc.setValueCaseSensitive(true);
  dsc.addSubstitution("foo", "bar");
  dsc.addSubstitution("bar", "420");
  dsc.setUserPreParser(userPreparser);
  rst = Parrot::Restriction(-  1,   1, Parrot::RestrictionViolationPolicy::Exception, "QTY only defined on interval [-1:+1]");
  dsc.addRestriction(rst);
  rst = Parrot::Restriction(-.01, .01, Parrot::RestrictionViolationPolicy::Warning,   "QTY very close to zero");
  dsc.addRestriction(rst);
  std::cout << dsc.to_string() << std::endl;

  std::cout << "reset:" << std::endl;
  dsc.reset();
  std::cout << "Type ID: " << dsc.getTypeID() << std::endl;
  std::cout << dsc.to_string() << std::endl;

  std::cout << "CTor with explicit type:" << std::endl;
  dsc = Parrot::Descriptor("foo bar", Parrot::ValueType::Integer);
  std::cout << "Type ID: " << dsc.getTypeID() << std::endl;
  std::cout << dsc.to_string() << std::endl;

  std::cout << "CTor with implicit type:" << std::endl;
  dsc = Parrot::Descriptor("foo bar", "const char *");
  std::cout << "Type ID: " << dsc.getTypeID() << std::endl;
  std::cout << dsc.to_string() << std::endl;

  std::cout << "CTor with implicit type:" << std::endl;
  dsc = Parrot::Descriptor("foo bar", "std::string"s);
  std::cout << "Type ID: " << dsc.getTypeID() << std::endl;
  std::cout << dsc.to_string() << std::endl;

  std::cout << "CTor with implicit type:" << std::endl;
  dsc = Parrot::Descriptor("foo", std::vector<std::string>());
  std::cout << "Type ID: " << dsc.getTypeID() << std::endl;
  std::cout << dsc.to_string() << std::endl;

  std::cout << "CTor with implicit type:" << std::endl;
  dsc = Parrot::Descriptor("foo", {"string list"});
  std::cout << "Type ID: " << dsc.getTypeID() << std::endl;
  std::cout << dsc.to_string() << std::endl;

  std::cout << "CTor with implicit type:" << std::endl;
  dsc = Parrot::Descriptor("foo", {true, false, true});
  std::cout << "Type ID: " << dsc.getTypeID() << std::endl;
  std::cout << dsc.to_string() << std::endl;
}
// .......................................................................... //
void unittest_Descriptor_make() {
  BCG::writeBoxed("Testing the Descriptor Class make* functions", {BCG::ConsoleColors::FORE_YELLOW});

  Parrot::Descriptor dsc;

  std::cout << "makeRanged, explicit:" << std::endl;
  dsc.makeRanged("foo bar", Parrot::ValueType::IntegerList, -1, 1);
  std::cout << dsc.to_string() << std::endl;

  std::cout << "makeRanged, implicit:" << std::endl;
  dsc.makeRanged<std::vector<double>>("foo bar", {1., -1.}, -1, 1);
  std::cout << dsc.to_string() << std::endl;

  std::cout << "makeRanged, explicit, invalid type (boolean): " << std::flush;
  try {dsc.makeListboundAftParse("foo bar", Parrot::ValueType::Boolean, std::vector<bool>({true, false}));}
  catch (std::exception & e) {std::cout << "prevented" << std::endl;}
  std::cout << std::endl;

  std::cout << "makeRanged, explicit, invalid type (string): " << std::flush;
  try {dsc.makeRanged("foo bar", Parrot::ValueType::String, -1, 1);}
  catch (std::exception & e) {std::cout << "prevented" << std::endl;}
  std::cout << std::endl;



  std::cout << "makeListboundPreParse, explicit:" << std::endl;
  dsc.makeListboundPreParse("foo bar", Parrot::ValueType::RealList, {"-1", "0", "1"});
  std::cout << dsc.to_string() << std::endl;

  std::cout << "makeListboundPreParse, implicit:" << std::endl;
  dsc.makeListboundPreParse("foo bar", std::vector<int>({1, 2, 3}), {"-1", "0", "1"}, true);
  std::cout << dsc.to_string() << std::endl;

  std::cout << "makeListboundPreParse, implicit, invalid type (boolean): " << std::flush;
  try {dsc.makeListboundPreParse("foo bar", std::vector<bool>({true, false}), {"active", "inactive"}, true);}
  catch (std::exception & e) {std::cout << "prevented" << std::endl;}
  std::cout << std::endl;



  std::cout << "makeListboundAftParse, explicit:" << std::endl;
  dsc.makeListboundAftParse("foo bar", Parrot::ValueType::RealList, std::vector<double>({-1, 0, 1}));
  std::cout << dsc.to_string() << std::endl;

  std::cout << "makeListboundAftParse, implicit: " << std::endl;
  dsc.makeListboundAftParse("foo bar", 10, std::vector<int>({-1, 0, 1}), true);
  std::cout << dsc.to_string() << std::endl;

  std::cout << "makeListboundAftParse, implicit:" << std::endl;
  dsc.makeListboundAftParse("foo bar", std::vector<double>({0.5}), std::vector<double>({-1, 0, 1}), true);
  std::cout << dsc.to_string() << std::endl;

  std::cout << "makeListboundAftParse, explicit, mismatched type (real list vs. int list): " << std::flush;
  try {dsc.makeListboundAftParse("foo bar", Parrot::ValueType::RealList, std::vector<int>({-1, 0, 1}));}
  catch (std::exception & e) {std::cout << "prevented" << std::endl;}
  std::cout << std::endl;

  std::cout << "makeListboundAftParse, explicit, mismatched type (real vs. int list): " << std::flush;
  try {dsc.makeListboundAftParse("foo bar", Parrot::ValueType::Real, std::vector<int>({-1, 0, 1}));}
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
  dsc.makeUserboundPreParse("foo bar", Parrot::ValueType::BooleanList, manualStringValidation);
  std::cout << dsc.to_string() << std::endl;

  std::cout << "makeUserboundPreParse, implicit:" << std::endl;
  dsc.makeUserboundPreParse("foo bar", true, manualStringValidation);
  std::cout << dsc.to_string() << std::endl;



  std::cout << "makeUserboundAftParse, explicit:" << std::endl;
  dsc.makeUserboundAftParse<int>("foo bar", Parrot::ValueType::Integer, manualIntValidation);
  std::cout << dsc.to_string() << std::endl;

  std::cout << "makeUserboundAftParse, implicit:" << std::endl;
  dsc.makeUserboundAftParse<int>("foo bar", 1, manualIntValidation);
  std::cout << dsc.to_string() << std::endl;

  std::cout << "makeUserboundAftParse, explicit, mismatched types (boolean list vs. string): " << std::flush;
  try {dsc.makeUserboundAftParse<std::string>("foo bar", Parrot::ValueType::BooleanList, manualStringValidation);}
  catch (std::exception & e) {std::cout << "prevented" << std::endl;}
  std::cout << std::endl;

}

// ========================================================================== //
// main

template<Parrot::ValueType> struct getType {};

template<> struct getType<Parrot::ValueType::Integer> {using vt = int;};

template <int n>
using X = int[n];

int main () {
  BCG::init();

  BCG::writeBoxed("SETTINGS PACKAGE UNIT TEST", {BCG::ConsoleColors::FORE_GREEN}, 80, '=', '#', '#');

//   unittest_convenience();
//   unittest_Restriction();
//   unittest_Descriptor_primitive();
  unittest_Descriptor_make();

  std::cout << std::endl;
  BCG::writeBoxed("ALL DONE -- HAVE A NICE DAY!", {BCG::ConsoleColors::FORE_GREEN}, 80, '=', '#', '#');

  X<3> a;
  std::cout << a << "\t" << a[0] << std::endl;

  getType<Parrot::ValueType::Integer>::vt x = 3;
  std::cout << x << std::endl;
}
