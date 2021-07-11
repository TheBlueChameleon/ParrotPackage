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
  // types

  /**
   * @brief represents the data type associated with a keyword.
   *
   * To get a human-readable representation of a \c Parrot::ValueTypeID, use
   *  \c Parrot::valueTypeName()
   *
   *
   * <table>
   *  <tr><th>ValueTypeID     <th> interpret the keyword as
   *  <tr><td>\c String       <td> \c std::string
   *  <tr><td>\c Integer      <td> <tt>long long int</tt>
   *  <tr><td>\c Real         <td> \c double
   *  <tr><td>\c Boolean      <td> \c bool
   *  <tr><td>\c StringList   <td> \c std::vector<std::string>>
   *  <tr><td>\c IntegerList  <td> <tt>std::vector&lt;long long int&gt;</tt>
   *  <tr><td>\c RealList     <td> \c std::vector<double>
   *  <tr><td>\c BooleanList  <td> \c std::vector<bool>
   * </table>
   *
   * The corresponding C++ types will be invoked by means of the PARROT_TYPE()
   * macro.
   */
  enum class ValueTypeID {
    String,
    Integer,
    Real,
    Boolean,
    StringList,
    IntegerList,
    RealList,
    BooleanList
  };
  // ........................................................................ //

  /**
   * @brief translates a Parrot::ValueTypeID() back into its corresponding C++
   *    type
   *
   * Example:
   * @code
   * Parrot::ValueType<Parrot::ValueTypeID::RealList>::value_type list_of_doubles = {2.71, 3.14};
   * @endcode
   *
   * @note the template parameter has to be a compile time constant.
   */
  template<Parrot::ValueTypeID> struct ValueType {};
  template<> struct ValueType<Parrot::ValueTypeID::String     > {using value_type = std::string;};
  template<> struct ValueType<Parrot::ValueTypeID::Integer    > {using value_type = long long int;};
  template<> struct ValueType<Parrot::ValueTypeID::Real       > {using value_type = double;};
  template<> struct ValueType<Parrot::ValueTypeID::Boolean    > {using value_type = bool;};
  template<> struct ValueType<Parrot::ValueTypeID::StringList > {using value_type = std::vector<std::string>;};
  template<> struct ValueType<Parrot::ValueTypeID::IntegerList> {using value_type = std::vector<long long int>;};
  template<> struct ValueType<Parrot::ValueTypeID::RealList   > {using value_type = std::vector<double>;};
  template<> struct ValueType<Parrot::ValueTypeID::BooleanList> {using value_type = std::vector<bool>;};
  // ........................................................................ //

  /**
   * @brief convenience macro for easier use of the Parrot::ValueType macro.
   *
   * Example:
   * @code
   * PARROT_TYPE<Parrot::ValueTypeID::RealList> list_of_doubles = {2.71, 3.14};
   * @endcode
   */
# define PARROT_TYPE(valueTypeID) Parrot::ValueType<valueTypeID>::value_type
  // ........................................................................ //

  extern const std::string TypeIDString_String;
  extern const std::string TypeIDString_Integer;
  extern const std::string TypeIDString_Real;
  extern const std::string TypeIDString_Boolean;

  extern const std::string TypeIDString_StringList;
  extern const std::string TypeIDString_IntegerList;
  extern const std::string TypeIDString_RealList;
  extern const std::string TypeIDString_BooleanList;
  // ........................................................................ //


  /**
   * @todo RestrictionValueTypeID
   */
  enum class RestrictionValueTypeID {
    None,
    String,
    Integer,
    Real,
    Numeric,
    Boolean
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
   *  <tr><td>\c Range          <td> \c Integer, \c Real, \c IntegerList and
   *                                  \c RealList
   *                                              <td> Allow all values between
   *                                                    an upper and a lower
   *                                                    boundary.<br>
   *                                                   May only be used as
   *                                                    aftParseRestriction
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
   *  <tr><th>RestrictionViolationPolicy <th>Effect
   *  <tr><td>\c Exception               <td>\c throw a <tt>Parrot::RestrictionViolationError</tt>
   *  <tr><td>\c Warning                 <td>utter a warning via stderr
   *  <tr><td>\c WarningRevert           <td>utter a warning via stderr and
   *                                          revert to the default value
   * </table>
   */
  enum class RestrictionViolationPolicy {
    Exception,
    Warning,
    WarningRevert
  };
  
  // ======================================================================== //
  // error classes
  
  /** @brief Error type thrown if a value for a keyword does not meet the
   *    specificationsas given by a Parrot::Restriction
   *
   *  @todo specify more Parrot errors
   */
  class RestrictionViolationError : public std::exception {};
  
  // ======================================================================== //
  // lookups
  
  //! @brief list of strings that are evaluated as \c true
  extern const std::vector<std::string> defaultBooleanTextTrue;
  //! @brief list of strings that are evaluated as \c false
  extern const std::vector<std::string> defaultBooleanTextFalse;
  
  /**
   * @brief returns a human readable string to a \c Parrot::ValueTypeID()
   *
   * Implements a simple lookup.
   *
   * @returns
   * <table>
   *  <tr><th>ValueTypeID     <th>return value
   *  <tr><td>\c String       <td>string
   *  <tr><td>\c Integer      <td>integer
   *  <tr><td>\c Real         <td>real value
   *  <tr><td>\c Boolean      <td>boolean
   *  <tr><td>\c StringList   <td>list of strings
   *  <tr><td>\c IntegerList  <td>list of integers
   *  <tr><td>\c RealList     <td>list of real values
   *  <tr><td>\c BooleanList  <td>list of booleans
   *  <tr><td>(otherwise)     <td>(invalid state)
   * </table>
   */
  const std::string valueTypeName                 (const ValueTypeID                  & T);

  /**
   * @brief returns a human readable string to a \c Parrot::RestrictionType()
   *
   * Implements a simple lookup.
   *
   * @returns
   *<table>
   *  <tr><th>RestrictionType  <th>return value
   *  <tr><td>\c None          <td>none
   *  <tr><td>\c AllowedList   <td>list of allowed values
   *  <tr><td>\c ForbiddenList <td>list of forbidden values
   *  <tr><td>\c Range         <td>range
   *  <tr><td>\c Function      <td>user defined verification function
   *  <tr><td>(otherwise)      <td>(invalid state)
   * </table>
   */
  const std::string restrictionTypeName           (const RestrictionType            & T);

  /**
   * @brief returns a human readable string to a \c Parrot::RestrictionViolationPolicy()
   *
   * Implements a simple lookup.
   *
   * @returns
   *<table>
   *  <tr><th>RestrictionViolationPolicy <th>return value
   *  <tr><td>\c Exception     <td>throw a RestrictionViolationError
   *  <tr><td>\c Warning       <td>utter a warning via stderr
   *  <tr><td>\c WarningRevert <td>utter a warning to stderr and revert to the default value
   *  <tr><td>(otherwise)      <td>(invalid state)
   * </table>
   */
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
  bool isTypeCompatibleWithValidityList(const ValueTypeID & valueType, const ValueTypeID & listType);



  // ======================================================================== //
  // type interpreters

  /**
   * @brief returns a \c Parrot::ValueTypeID() for an arbitrary expression
   *
   * The following table tells which \c Parrot::ValueTypeID()s are constructed
   * from which C++ data types:
   *
   * <table>
   *  <tr><th>ValueTypeID   <th> C++ types
   *  <tr><td>String      <td> \c std::string, <tt>char *</tt>, <tt>char[]</tt>
   *                            and their \c const counterparts
   *  <tr><td>Integer     <td> Integral types (\c char, \c short, \c int,
   *                            \c long, <tt>long long</tt>, as well as their
   *                            \c unsigned and \c const counterparts)
   *  <tr><td>Real        <td> floating point values (\c float, \c double,
   *                            <tt>long double</tt> as well as their
   *                            \c const counterparts)
   *  <tr><td>Boolean     <td> \c bool (explicitly only that type, for
   *                            simplicity)
   *  <tr><td>StringList  <td> Any type, from which a
   *                            \c std::vector<std::string> can be constructed
   *  <tr><td>IntegerList <td> Any type, from which a \c std::vector<int> can be
   *                            constructed
   *  <tr><td>RealList    <td> Any type, from which a \c std::vector<double> can
   *                            be constructed
   *  <tr><td>BooleanList <td> Any type, from which a \c std::vector<bool> can
   *                            be constructed
   * </table>
   *
   * @throws std::invalid_argument for all types not in the table.
   */
  template<typename T>
  constexpr ValueTypeID valueTypeIDOf(const T & x);

  /**
   * @brief returns a \c Parrot::ValueTypeID() for an arbitrary
   *  \c std::initializer_list
   *
   * To avoid ambiguous template instantiations, this convenience function
   * converts an \c std::initializer_list<T> into a \c std::vector<T> before
   * handing it over to \c valueTypeOf() and forwarding that call's return value.
   */
  template<typename T>
  constexpr ValueTypeID valueTypeIDOf(const std::initializer_list<T> & x);

  /**
   * @brief returns a textual representation of any compatible expression.
   *
   * @param x an arbitrary-type value
   * @param T a \c Parrot::ValueTypeID() into which \c x should be cast before
   *  computing its string representation
   *
   * The generated text follows these rules:
   *
   * <table>
   *  <tr><th>ValueTypeID    <th> Output
   *  <tr><td>\c String      <td> The \c std::string as-is
   *  <tr><td>\c Integer     <td> <tt>std::to_string(intvalue)</tt>
   *  <tr><td>\c Real        <td> <tt>std::to_string(realvalue)</tt>
   *  <tr><td>\c Boolean     <td> \c "true" or \c "false"
   *  <tr><td>\c StringList  <td> Output of \c BCG::vector_to_string()
   *  <tr><td>\c IntegerList <td> Output of \c BCG::vector_to_string()
   *  <tr><td>\c RealList    <td> Output of \c BCG::vector_to_string()
   *  <tr><td>\c BooleanList <td> A string comprising of \c 1 for every \c true and
   *                           a \c o for every \c false
   * </table>
   */
  const std::string getAnyText(const std::any & x, const ValueTypeID & T);

  /**
   * @brief shortcut to
   *    <tt>Parrot::getAnyText(const std::any & x, const ValueTypeID & T)</tt>
   */
  const std::string getAnyText(const std::any & x);

  //! @}
}

// ========================================================================== //
// template implementations

#include "Parrot/Definitions.tpp"

// ========================================================================== //

#endif
