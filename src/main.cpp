// ========================================================================== //
// dependencies

// STL
#include <iostream>
#include <iomanip>

#include <string>
using namespace std::string_literals;
#include <any>

#include <initializer_list>
#include <vector>
#include <iterator>

#include <chrono>
#include <thread>

// own
#include "BCG.hpp"

#include "globals.hpp"
#include "Parrot.hpp"

// ========================================================================== //
// unittest globals

// -------------------------------------------------------------------------- //

void unittest_globals () {
  {
    BCG::writeBoxed("Testing the BCG console output functions", {BCG::ConsoleColors::FORE_YELLOW});
    std::cout << "as you can see, writeBoxed works pretty fine" << std::endl;

    BCG::writeWarning("not all output happens on STDOUT.");

    for (auto i = 0; i < 20; ++i) {
      BCG::idleAnimation("some text ");
      std::this_thread::sleep_for( std::chrono::milliseconds(100));
    }
    std::cout << "" << std::endl;
  }

  {
    BCG::writeBoxed("Testing the BCG random number", {BCG::ConsoleColors::FORE_YELLOW});

    std::cout << "truely random int           : " << BCG::trueRNG() << std::endl;
    std::cout << "seed, obtainted from trueRNG: " << BCG::seedRNG << std::endl;
    std::cout << "pseudorandom int            : " << BCG::PRNG() << std::endl;
    std::cout << std::endl;

    std::cout << "left column : random values between 0 and 1" << std::endl;
    std::cout << "right column: random values between 0 and 2pi" << std::endl;
    std::cout << std::setw(8) << std::fixed;
    for (int i = 0; i < 10; ++i) {
      std::cout << BCG::get_randPercentage() << "\t" << BCG::get_randPhase() << std::endl;
    }
    std::cout << std::defaultfloat;
  }

  {
    BCG::writeBoxed("Testing the BCG Vector utility functions", {BCG::ConsoleColors::FORE_YELLOW});

    auto        x = {1, 2, 3};
    auto        y = {4, 5, 6};
    std::vector a = x;
    std::vector b = y;

    std::cout << "source data:" << std::endl;
    std::cout << "x/a = " << BCG::vector_to_string(a) << std::endl;
    std::cout << "y/b = " << BCG::vector_to_string(b) << std::endl;
    std::cout << std::endl;

    std::cout << "vector specific concatenate: " << std::flush;
    std::cout << BCG::vector_to_string(BCG::concatenate(a, b)) << std::endl;

    std::cout << "generic concatenate        : " << std::flush;
    std::cout << BCG::vector_to_string(BCG::concatenate(
      x.begin(), x.end(),
      y.begin(), y.end())
    ) << std::endl;

    std::cout << BCG::isTTY << std::endl;
  }

}

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
  std::cout << dsc.getTypeID() << std::endl;
  std::cout << dsc.to_string() << std::endl;

  std::cout << "CTor with explicit type:" << std::endl;
  dsc = Parrot::Descriptor("foo bar", Parrot::ValueType::Integer);
  std::cout << dsc.getTypeID() << std::endl;
  std::cout << dsc.to_string() << std::endl;

  std::cout << "CTor with implicit type:" << std::endl;
  dsc = Parrot::Descriptor("foo bar", "const char *");
  std::cout << dsc.getTypeID() << std::endl;
  std::cout << dsc.to_string() << std::endl;

  std::cout << "CTor with implicit type:" << std::endl;
  dsc = Parrot::Descriptor("foo bar", "std::string"s);
  std::cout << dsc.getTypeID() << std::endl;
  std::cout << dsc.to_string() << std::endl;

  std::cout << "CTor with implicit type:" << std::endl;
  dsc = Parrot::Descriptor("foo", std::vector<std::string>());
  std::cout << dsc.getTypeID() << std::endl;
  std::cout << dsc.to_string() << std::endl;

  std::cout << "CTor with implicit type:" << std::endl;
  dsc = Parrot::Descriptor("foo", {"string list"});
  std::cout << dsc.getTypeID() << std::endl;
  std::cout << dsc.to_string() << std::endl;

  std::cout << "CTor with implicit type:" << std::endl;
  dsc = Parrot::Descriptor("foo", {true, false, true});
  std::cout << dsc.getTypeID() << std::endl;
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


int main () {
#ifdef BCG_MATHS
  std::cout << "BCG MATHS available" << std::endl;
#else
  std::cout << "BCG MATHS not available" << std::endl;
#endif

  BCG::init();

  BCG::writeBoxed("SETTINGS PACKAGE UNIT TEST", {BCG::ConsoleColors::FORE_GREEN}, 80, '=', '#', '#');

  unittest_globals ();
//   unittest_convenience();
//   unittest_Restriction();
//   unittest_Descriptor_primitive();
//   unittest_Descriptor_make();

  std::cout << std::endl;
  BCG::writeBoxed("ALL DONE -- HAVE A NICE DAY!", {BCG::ConsoleColors::FORE_GREEN}, 80, '=', '#', '#');
}
