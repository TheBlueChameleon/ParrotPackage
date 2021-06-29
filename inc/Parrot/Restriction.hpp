/* TODO: File descrption
 * 
 */

#ifndef SETTINGS_RESTRICTION_HPP
#define SETTINGS_RESTRICTION_HPP

// ========================================================================== //
// dependencies

// STL
#include <string>
#include <vector>
#include <any>
#include <tuple>

#include <functional>

// own
#include "Parrot/Definitions.hpp"

// ========================================================================== //

namespace Parrot {
  
  // ======================================================================== //
  // class
  
  /**
   * @brief Represents one rule according to which a value to a keyword will be
   *  considered valid or invalid.
   *
   * A restriction is comprised of a \c preParseRestriction and a
   * \c aftParseRestriction. The Former will be evaluated on text level (i.e.
   * with strings as operands) while the latter will be evaluated after parsing
   * the line from the .ini file into its designated \c Parrot::ValueType().
   * Further, a \c restrictionViolationPolicy and a \c restrictionViolationText
   * are attached to the Parrot::Restriction, specifying whether to throw an
   * error or print a warning to stderr.
   *
   * If \c restrictionViolationPolicy is set to
   * \c Parrot::RestrictionViolationPolicy::Warning, then the
   * \c restrictionViolationText will be output via \c BCG::writeWarning().
   * Otherwise, the text will be used in the throwtext.
   *
   * By default, a \c Parrot::Restriction is configured to throw an exception
   * with the \c restrictionViolationText <tt>invalid line</tt>
   *
   * An instance of Parrot::Restriction may make use of both, the
   * \c preParseRestriction and the \c aftParseRestriction at the same time.
   * Likewise, either of them can be set to \c Parrot::RestrictionType::None.
   * See \c Parrot::RestrictionType() for details.
   */

  //! @todo make a CTor with a string argument, creating a valid characters check function

  class Restriction {
  private:
    RestrictionType preParseRestrictionType = RestrictionType::None;
    std::any        preParseRestriction;
    
    RestrictionType aftParseRestrictionType = RestrictionType::None;
    std::any        aftParseRestriction;
    
    RestrictionViolationPolicy  restrictionViolationPolicy = RestrictionViolationPolicy::Exception;
    std::string                 restrictionViolationText   = "invalid line";
    
  public:
    // ---------------------------------------------------------------------- //
    // CTors
    
    Restriction() = default;
    
    /**
     * @brief constructs an empty \c Parrot::Restriction (i.e. with no
     *    \c Parrot::RestrictionType() attached) and allows to directly set the
     *    \c Parrot::RestrictionViolationPolicy() and the
     *    \c restrictionViolationText.
     *
     * @param restrictionViolationPolicy an instance of
     *    \c Parrot::RestrictionViolationPolicy() indicating whether a warning
     *    should be printed to stderr (by calling \c BCG::writeWarning()) or
     *    whether a \c Parrot::RestrictionViolationError() should be thrown.
     * @param The text to be output when the Restriction is not met by the
     *    value in the \c .ini file.
     *
     * By default, a \c Parrot::Restriction is configured to throw an exception
     * with the \c restrictionViolationText <tt>invalid line</tt>
     */
    Restriction(
      RestrictionViolationPolicy restrictionViolationPolicy,
      const std::string & restrictionViolationText = "invalid line"
    );
    

    /**
     * @brief constructs a \c Parrot::Restriction with a \c aftParseRestriction
     * checking whether the value was between \c min and \c max by calling
     * \c setAftParseRange().
     *
     * @param min the smallest acceptable value for the keyword being
     *    specified
     * @param max the largest acceptable value for the keyword being
     *    specified
     * @param restrictionViolationPolicy an instance of
     *    \c Parrot::RestrictionViolationPolicy() indicating whether a warning
     *    should be printed to stderr (by calling \c BCG::writeWarning()) or
     *    whether a \c Parrot::RestrictionViolationError() should be thrown.
     * @param restrictionViolationText The text to be output when the
     *    restriction is not met by the value in the \c .ini file.
     *
     * @attention *Parrot* does not check whether <tt> min < max</tt>!
     */
    Restriction(
      double min, double max,
      RestrictionViolationPolicy restrictionViolationPolicy = RestrictionViolationPolicy::Exception,
      const std::string & restrictionViolationText = "value out of bounds"
    );

    /**
     * @brief constructs a \c Parrot::Restriction with a validationlist, \ie a
     *    list of strings that are either the only allowed values
     *    (\c RestrictionType::AllowedList) or forbidden values for the keyword
     *    (\c RestrictionType::ForbiddenList).<br>
     *    The restrictions set with this constructor are used for the
     *    \c preParseRestriction.
     *
     * @param list the list of strings to use as a validationlist.
     * @param ForbiddenList indicates whether the passed \c list should be
     *    seen as a \c RestrictionType::AllowedList (default) or as a
     *    \c RestrictionType::ForbiddenList (behaviour when \c list is set to
     *    \c false. See \c Parrot::RestrictionType() for details.
     * @param restrictionViolationPolicy an instance of
     *    \c Parrot::RestrictionViolationPolicy() indicating whether a warning
     *    should be printed to stderr (by calling \c BCG::writeWarning()) or
     *    whether a \c Parrot::RestrictionViolationError() should be thrown.
     * @param restrictionViolationText The text to be output when the
     *    restriction is not met by the value in the \c .ini file.
     */
    Restriction(
      const std::vector<std::string> & list,
      bool forbiddenList = false,
      RestrictionViolationPolicy restrictionViolationPolicy = RestrictionViolationPolicy::Exception,
      const std::string & restrictionViolationText = "value not allowed"
    );

    /**
     * @brief constructs a \c Parrot::Restriction with a user defined validation
     *    function.
     *    The restrictions set with this constructor are used for the
     *    \c preParseRestriction.
     *
     * @param uFunc a function taking a <tt>std::string &</tt> and returning a
     *    \c bool that indicates whether a value should be considered valid
     *    (return value \c true) or invalid (return value \c false)
     * @param restrictionViolationPolicy an instance of
     *    \c Parrot::RestrictionViolationPolicy() indicating whether a warning
     *    should be printed to stderr (by calling \c BCG::writeWarning()) or
     *    whether a \c Parrot::RestrictionViolationError() should be thrown.
     * @param restrictionViolationText The text to be output when the
     *    restriction is not met by the value in the \c .ini file.
     */
    Restriction(
      const std::function<bool (const std::string &)> & uFunc,
      RestrictionViolationPolicy restrictionViolationPolicy = RestrictionViolationPolicy::Exception,
      const std::string & restrictionViolationText = "value not allowed"
    );
    

    // ---------------------------------------------------------------------- //
    // Getters

    /**
     * @brief returns the current \c Parrot::RestrictionType applied before
     *    parsing.
     */
    RestrictionType   getPreParseRestrictionType() const;

    /**
     * @brief returns the data against which a value is checked according to the
     *    rules implied by the state of the \c preParseRestriction (cf.
     *    \c Parrot::getPreParseRestrictionType()).
     *
     * The return value will be a \c std::any instance that can be cast into
     * various data types, depending on the value of \c preParseRestrictionType:
     *
     * <table>
     *  <tr><th>RestrictionType  <th> \c std::any_cast type parameter
     *  <tr><td>\c None          <td> <em>(none -- returned object will have no
     *                                value)</em>
     *  <tr><td>\c AllowedList   <td> \c std::vector<std::string>
     *  <tr><td>\c ForbiddenList <td> \c std::vector<std::string>
     *  <tr><td>\c Range         <td> <em>(not applicable for
     *                                <tt>preParseRestriction</tt>s)</em>
     *  <tr><td>\c Function      <td> <tt>const std::function&lt;bool (const std::string &)&gt; &</tt>
     * </table>
     */
    const std::any &  getPreParseRestriction    () const;
    


    /**
     * @brief returns the current \c Parrot::RestrictionType applied after
     *    parsing.
     */
    RestrictionType   getAftParseRestrictionType() const;

    /**
     * @brief returns the data against which a value is checked according to the
     *    rules implied by the state of the \c aftParseRestriction (cf.
     *    \c Parrot::aftPreParseRestrictionType()).
     *
     * The return value will be a \c std::any instance that can be cast into
     * various data types, depending on the value of \c aftParseRestrictionType:
     *
     * <table>
     *  <tr><th>RestrictionType  <th> \c std::any_cast type parameter
     *  <tr><td>\c None          <td> <em>(none -- returned object will have no
     *                                value)</em>
     *  <tr><td>\c AllowedList   <td> \c std::vector<T>
     *  <tr><td>\c ForbiddenList <td> \c std::vector<T>
     *  <tr><td>\c Range         <td> <tt>std::pair&lt;double, double&gt;</tt>
     *  <tr><td>\c Function      <td> <tt>const std::function&lt;bool (const std::string &)&gt; &</tt>
     * </table>
     */
    const std::any &  getAftParseRestriction    () const;
    
    
    /**
     * @brief returns the boundaries between which a parsed value should be
     *    considered valid as a <tt>std::pair&lt;double, double&gt;</tt>, if the
     *    \c aftParseRestriction is set to \c Parrot::RestrictionType::Range.
     *    Otherwise, a \c std::runtime_error is thrown.
     */
    const std::pair<double, double>                 getAftParseRange() const;
    
    /**
     * @brief returns the validation list according to which an unparsed line
     *    in the .ini file should be considered valid as a
     *    \c std::vector<std::string>, if the \c aftParseRestriction is set to
     *    \c Parrot::RestrictionType::AllowedList or
     *    \c Parrot::RestrictionType::ForbiddenList, respectively.
     *    Otherwise, a \c std::runtime_error is thrown.
     */
    const std::vector<std::string>                  getPreParseValidationList() const;
    template<typename T>
    const std::vector<T>                            getAftParseValidationList () const;
    
    const std::function<bool (const std::string &)> getPreParseValidationFunction() const;
    template<typename T>
    const std::function<bool (const T &)>           getAftParseValidationFunction () const;
    
    RestrictionViolationPolicy  getRestrictionViolationPolicy() const;
    const std::string &         getRestrictionViolationText  () const;
    
    // ---------------------------------------------------------------------- //
    // Setters
    
    void reset();
    void resetPreParseRestriction();
    void resetAftParseRestriction();
    
    void setAftParseRange(const double min, const double max);
    
    void setPreParseValidationList(const std::vector<std::string> & list, bool forbiddenList = false);
    template<typename T>
    void setAftParseValidationList(const std::vector<T>           & list, bool forbiddenList = false);
    
    void setPreParseValidationFunction(const std::function<bool (const std::string &)> & uFunc);
    template<typename T>
    void setAftParseValidationFunction(const std::function<bool (const T &)> &           uFunc);
    
    void setRestrictionViolationPolicy (RestrictionViolationPolicy restrictionViolationPolicy, const std::string & text);
    void setViolationWarningText  (const std::string & text);
    void setViolationExceptionText(const std::string & text);
    
    // ---------------------------------------------------------------------- //
    // Representation
    
    std::string to_string() const;
  };
}

// ========================================================================== //
// template implementations

#include "Parrot/Restriction.tpp"

// ========================================================================== //
#endif
