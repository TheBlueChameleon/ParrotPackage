/**
 * @defgroup Parrot_Definitions Parrot Definitions
 *
 * @brief Filetype Definitions shared between the components of the \em Parrot
 * Package, as well as convenience functions to handle them.
 */


#ifndef SETTINGS_DEFINITIONS_HPP
#define SETTINGS_DEFINITIONS_HPP

// ========================================================================== //
// dependencies

// STL
#include <exception>

#include <vector>
#include <string>
#include <any>
#include <initializer_list>

// ========================================================================== //

namespace Parrot {
  //! @addtogroup Parrot_Definitions
  //! @{

  // ======================================================================== //
  // enums

  /**
   * @brief represents the data type associated with a keyword.
   *
   * <table>
   *  <tr><th>ValueType       <th> interpret the keyword as
   *  <tr><td>\c String       <td> \c std::string
   *  <tr><td>\c Integer      <td> \c int
   *  <tr><td>\c Real         <td> \c double
   *  <tr><td>\c Boolean      <td> \c bool
   *  <tr><td>\c Stringlist   <td> \c std::vector<std::string>>
   *  <tr><td>\c IntegerList  <td> \c std::vector<int>
   *  <tr><td>\c RealList     <td> \c std::vector<double>
   *  <tr><td>\c BooleanList  <td> \c std::vector<bool>
   * </table>
   */
  enum class ValueType {
    String,
    Integer,
    Real,
    Boolean,
    StringList,
    IntegerList,
    RealList,
    BooleanList
  };
  
  // ------------------------------------------------------------------------ //
  
  /**
   * @brief specifies the kind or restriction applied to a keyword.
   *
   * <table>
   *  <tr><th>RestrictionType   <th> Applies to   <th> Meaning
   *  <tr><td>\c None           <td> All          <td> Allow any value
   *  <tr><td>\c AllowedList    <td> All but \c Boolean and \c BooleanList
   *                                              <td> Allow only values that
   *                                                    are in a given list
   *  <tr><td>\c ForbiddenList  <td> All but \c Boolean and \c BooleanList
   *                                              <td> Allow all values except
   *                                                    those which are in a
   *                                                    given list
   *  <tr><td>\c Range          <td> All but \c Boolean and \c BooleanList
   *                                              <td> Allow all values between
   *                                                    an upper and a lower
   *                                                    boundary
   *  <tr><td>\c Function       <td> All          <td> Allow all values that,
   *                                                    when passed to a user
   *                                                    defined function, return
   *                                                    \c true
   * </table>
   *
   * For \c AllowedList and \c ForbiddenList see also
   * Parrot::isTypeCompatibleWithValidityList()
   */
  enum class RestrictionType {
    None,
    AllowedList,
    ForbiddenList,
    Range,
    Function
  };
  
  // ------------------------------------------------------------------------ //
  
  /**
   * @brief specifies what reaction is triggered when the value for a keyword
   *  does not meet its specifications as given by a Parrot::Restriction
   *
   * <table>
   *  <tr><th>RestrictionViolationPolicy  <th> Effect
   *  <tr><td>Exception                   <td> \c throw a \c Parrot::RestrictionViolationError
   *  <tr><td>Warning                     <td> print a warning to stderr
   * </table>
   */
  enum class RestrictionViolationPolicy {
    Exception,
    Warning
  };
  
  // ======================================================================== //
  // structs and classes
  
  /** @brief Error type thrown if a value for a keyword does not meet the
   *    specificationsas given by a Parrot::Restriction
   */
  class RestrictionViolationError : public std::exception {};
  
  // ======================================================================== //
  // lookups
  
  //! @brief list of strings that are evaluated as \c true
  extern const std::vector<std::string> defaultBooleanTextTrue;
  //! @brief list of strings that are evaluated as \c false
  extern const std::vector<std::string> defaultBooleanTextFalse;
  
  //! @brief returns a human readable string to a \c Parrot::ValueType()
  const std::string valueTypeName                 (const ValueType                  & T);
  //! @brief returns a human readable string to a \c Parrot::RestrictionType()
  const std::string restrictionTypeName           (const RestrictionType            & T);
  //! @brief returns a human readable string to a \c Parrot::RestrictionViolationPolicy()
  const std::string restrictionViolationPolicyName(const RestrictionViolationPolicy & T);

  /**
   * @brief returns \c true if a \c listType can be used as a list of (in)valid
   * values for a Parrot::Restriction, given the \c valueType associated with
   * the keyword
   *
   * A \c listType must name a list in order to be used as a \c AllowedList or
   * \c ForbiddenList (cf. \c Parrot::RestrictionType()).
   * Further, the elements in the \c AllowedList or \c ForbiddenList must be
   * comparable to the value of the keyword. Finally, it does not make sense to
   * restrict Booleans any further, hence they are never allowed as \c valueType
   *
   * In other words, the return values of this function are given by the table:
   * <table>
   *  <tr><td><td><th colspan="8"> \c valueType
   *  <tr><td><td>             <th>\c String <th>\c Integer <th>\c Real   <th>\c Boolean <th>\c StringList <th>\c IntegerList <th>\c RealList  <th>\c BooleanList
   *  <tr><th rowspan="8"> \c listType
   *      <th>\c String        <td>\c false  <td>\c false   <td>\c false  <td>\c false   <td>\c false      <td>\c false       <td>\c false     <td>\c false
   *  <tr><th>\c Integer       <td>\c false  <td>\c false   <td>\c false  <td>\c false   <td>\c false      <td>\c false       <td>\c false     <td>\c false
   *  <tr><th>\c Real          <td>\c false  <td>\c false   <td>\c false  <td>\c false   <td>\c false      <td>\c false       <td>\c false     <td>\c false
   *  <tr><th>\c Boolean       <td>\c false  <td>\c false   <td>\c false  <td>\c false   <td>\c false      <td>\c false       <td>\c false     <td>\c false
   *  <tr><th>\c StringList    <td>\c\b true <td>\c false   <td>\c false  <td>\c false   <td>\c\b true     <td>\c false       <td>\c false     <td>\c false
   *  <tr><th>\c IntegerList   <td>\c false  <td>\c\b true  <td>\c false  <td>\c false   <td>\c false      <td>\c\b true      <td>\c false     <td>\c false
   *  <tr><th>\c RealList      <td>\c false  <td>\c false   <td>\c\b true <td>\c false   <td>\c false      <td>\c false       <td>\c\b true    <td>\c false
   *  <tr><th>\c BooleanList   <td>\c false  <td>\c false   <td>\c false  <td>\c false   <td>\c false      <td>\c false       <td>\c false     <td>\c false
   * </table>
   */
  bool isTypeCompatibleWithValidityList(const ValueType & valueType, const ValueType & listType);

  // ======================================================================== //
  // type interpreters

  /**
   * @brief returns a \c Parrot::ValueType() for an arbitrary expression
   *
   * The following table tells which \c Parrot::ValueType()s are constructed
   * from which C++ data types:
   *
   * <table>
   *  <tr><th>ValueType   <th> C++ types
   *  <tr><td>String      <td> \c std::string, \c char *, \c char[] and their \c const counterparts
   *  <tr><td>Integer     <td> Integral types (\c char, \c short, \c int, \c long, <tt>long long</tt>, as well as their \c unsigned and \c const counterparts)
   *  <tr><td>Real        <td> floating point values (\c float, \c double, <tt>long double</tt> as well as their \c const counterparts)
   *  <tr><td>Boolean     <td> \c bool (explicitly only that type, for simplicity)
   *  <tr><td>StringList  <td> Any type, from which a \c std::vector<std::string> can be constructed
   *  <tr><td>IntegerList <td> Any type, from which a \c std::vector<int> can be constructed
   *  <tr><td>RealList    <td> Any type, from which a \c std::vector<double> can be constructed
   *  <tr><td>BooleanList <td> Any type, from which a \c std::vector<bool> can be constructed
   * </table>
   *
   * @throws std::invalid_argument for all types not in the table.
   */
  template<typename T>
  constexpr ValueType valueTypeOf(const T & x);

  /**
   * @brief returns a \c Parrot::ValueType() for an arbitrary
   *  \c std::initializer_list
   *
   * To avoid ambiguous template instantiations, this convenience function
   * converts an \c std::initializer_list<T> into a \c std::vector<T> before
   * handing it over to \c valueTypeOf() and forwarding that call's return value.
   */
  template<typename T>
  constexpr ValueType valueTypeOf(const std::initializer_list<T> & x);

  /**
   * @brief returns the STL typeID of an arbitrary expression
   *
   * An STL typeID is a sequence of characters that uniquely identify a type in
   * C++. Unfortunately, these strings are hard to read or to memorize. Look up
   * <a href="https://en.cppreference.com/w/cpp/types/type_info/name">the CPP reference entry for <tt>std::type_info::name</tt></a> for details
   */
  const std::string getTypeIDOf(const std::any & x);

  /**
   * @brief returns the STL typeID of an arbitrary \c std::initializer_list.
   *
   * As is the case with \c valueTypeOf(), resolving a \c std::initializer_list
   * can lead to ambiguities. This convenience function first converts the
   * \c std::initializer_list<T> to an \c std::vector<T> before returning its
   * <tt>std::type_info::name</tt>
   */
  template<typename T>
  const std::string getTypeIDOf(const std::initializer_list<T> & x);

  /**
   * @brief returns a textual representation of any compatible expression.
   *
   * @param x an arbitrary-type value
   * @param T a \c Parrot::ValueType() into which \c x should be cast before
   *  computing its string representation
   *
   * The generated text follows these rules:
   *
   * <table>
   *  <tr><th>ValueType      <th> Output
   *  <tr><td>\c String      <td> The \c std::string as-is
   *  <tr><td>\c Integer     <td> <tt>std::to_string(intvalue)</tt>
   *  <tr><td>\c Real        <td> <tt>std::to_string(realvalue)</tt>
   *  <tr><td>\c Boolean     <td> \c "true" or \c "false"
   *  <tr><td>\c StringList  <td> Output of \c BCG::vector_to_string()
   *  <tr><td>\c IntegerList <td> Output of \c BCG::vector_to_string()
   *  <tr><td>\c RealList    <td> Output of \c BCG::vector_to_string()
   *  <tr><td>\c BooleanList <td> A string comprising of \c + for every \c true and
   *                           a \c - for every \c false
   * </table>
   */
  const std::string getAnyText(const std::any & x, const ValueType & T);

  //! @}
}

// ========================================================================== //
// template implementations

#include "Parrot/Definitions.tpp"

// ========================================================================== //

#endif
