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
   *
   * The below parameters have the same meaning across all class members.
   *
   * @param policy the effect that is triggered when a read value does not
   *    fall within the specified range. See
   *    \c Parrot::RestrictionViolationPolicy() for details
   *
   * @param restrictionViolationText the text to be prompted with the event
   *    triggered when a read value does not fall within the specified range.
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
     * By default, a \c Parrot::Restriction is configured to throw an exception
     * with the \c restrictionViolationText <tt>invalid line</tt>
     */
    Restriction(
      RestrictionViolationPolicy restrictionViolationPolicy,
      const std::string & restrictionViolationText = "invalid line"
    );
    // ...................................................................... //

    /**
     * @brief constructs a \c Parrot::Restriction with a \c aftParseRestriction
     * checking whether the value was between \c min and \c max by calling
     * \c setAftParseRange().
     *
     * @param min the smallest acceptable value for the keyword being
     *    specified
     * @param max the largest acceptable value for the keyword being
     *    specified
     *
     * @attention *Parrot* does not check whether \c min &lt; \c max !
     */
    Restriction(
      double min, double max,
      RestrictionViolationPolicy restrictionViolationPolicy = RestrictionViolationPolicy::Exception,
      const std::string & restrictionViolationText = "value out of bounds"
    );
    // ...................................................................... //

    /**
     * @brief constructs a \c Parrot::Restriction with a validationlist, i.e. a
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
     */
    Restriction(
      const std::vector<std::string> & list,
      bool forbiddenList = false,
      RestrictionViolationPolicy restrictionViolationPolicy = RestrictionViolationPolicy::Exception,
      const std::string & restrictionViolationText = "value not allowed"
    );
    // ...................................................................... //

    /**
     * @brief constructs a \c Parrot::Restriction with a user defined validation
     *    function.
     *    The restrictions set with this constructor are used for the
     *    \c preParseRestriction.
     *
     * @param uFunc a function taking a <tt>std::string &</tt> and returning a
     *    \c bool that indicates whether a value should be considered valid
     *    (return value \c true) or invalid (return value \c false)
     */
    Restriction(
      const std::function<bool (const std::string &)> & uFunc,
      RestrictionViolationPolicy restrictionViolationPolicy = RestrictionViolationPolicy::Exception,
      const std::string & restrictionViolationText = "value not allowed"
    );
    

    // ---------------------------------------------------------------------- //
    // Getters

    /**
     * @brief returns the current \c Parrot::RestrictionType() applied before
     *    parsing.
     */
    RestrictionType   getPreParseRestrictionType() const;
    // ...................................................................... //

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
    // ...................................................................... //


    /**
     * @brief returns the current \c Parrot::RestrictionType() applied after
     *    parsing.
     */
    RestrictionType   getAftParseRestrictionType() const;
    // ...................................................................... //

    /**
     * @brief returns the data against which a value is checked according to the
     *    rules implied by the state of the \c aftParseRestriction (cf.
     *    \c Restriction::getAftParseRestrictionType()).
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
    // ...................................................................... //
    
    
    /**
     * @brief returns the boundaries between which a parsed value should be
     *    considered valid as a <tt>std::pair&lt;double, double&gt;</tt>, if the
     *    \c aftParseRestriction is set to \c Parrot::RestrictionType::Range.
     *    Otherwise, a \c std::runtime_error is thrown.
     *
     * @returns a <tt>std::pair&lt;double, double&gt; = {min, max}</tt>
     * @throws std::runtime_error if \c aftParseRestriction is not equal to
     *    \c Parrot::RestrictionType::Range.
     */
    const std::pair<double, double>                 getAftParseRange() const;
    // ...................................................................... //
    
    /**
     * @brief returns the validation list according to which an unparsed line
     *    in the .ini file should be considered valid as a
     *    \c std::vector<std::string>, if the \c aftParseRestriction is set to
     *    \c Parrot::RestrictionType::AllowedList or
     *    \c Parrot::RestrictionType::ForbiddenList, respectively.
     *    Otherwise, a \c std::runtime_error is thrown.
     *
     * @returns a \c std::vector<std::string> holding all allowed or forbidden
     *    values for the keyword being specified.
     * @throws std::runtime_error if \c aftParseRestriction is neither equal to
     *    \c Parrot::RestrictionType::AllowedList nor to
     *    \c Parrot::RestrictionType::ForbiddenList.
     */
    const std::vector<std::string>                  getPreParseValidationList() const;
    // ...................................................................... //

    /**
     * @brief returns the validation list according to which an parsed line in
     *    the .ini file should be considered valid as a \c std::vector<T> where
     *    \c T depends on the Parrot::ValueType().
     *
     * @returns a \c std::vector<T> containing all the allowed values for the
     *    keyword (if \c aftParseRestriction is equal to
     *    \c Parrot::RestrictionType::AllowedList) or all the forbidden values
     *    (if \c aftParseRestriction is equal to
     *    \c Parrot::RestrictionType::ForbiddenList), respectively.
     * @throws std::runtime_error if \c aftParseRestriction is neither equal to
     *    \c Parrot::RestrictionType::AllowedList nor to
     *    \c Parrot::RestrictionType::ForbiddenList.
     */
    template<typename T>
    const std::vector<T>                            getAftParseValidationList () const;
    // ...................................................................... //
    

    /**
     * @brief returns a \c std::function object representing the user defined
     *    function to be called to check validity of a keyword before parsing
     *    begins.
     *
     * @returns the \c std::function to be called to check for keyword validity,
     *    if \c preParseRestriction is equal to
     *    \c Parrot::RestrictionType::Function. Otherwise, throws an error.
     * @throws std::runtime_error if \c aftParseRestriction is not equal to
     *    \c Parrot::RestrictionType::Function.
     */
    const std::function<bool (const std::string &)> getPreParseValidationFunction() const;
    // ...................................................................... //

    /**
     * @brief returns a \c std::function object representing the user defined
     *    function to be called to check validity of a keyword after parsing
     *    to the Parrot::ValueType().
     *
     * @returns the \c std::function to be called to check for keyword validity,
     *    if \c preParseRestriction is equal to
     *    \c Parrot::RestrictionType::Function. Otherwise, throws an error.
     * @throws std::runtime_error if \c aftParseRestriction is not equal to
     *    \c Parrot::RestrictionType::Function.
     */
    template<typename T>
    const std::function<bool (const T &)>           getAftParseValidationFunction () const;
    // ...................................................................... //
    

    /**
     * @brief returns the Parrot::RestrictionViolationPolicy() applied when the
     *    described validity check is not passed.
     */
    RestrictionViolationPolicy  getRestrictionViolationPolicy() const;
    // ...................................................................... //

    /**
     * @brief returns the text that is output when the described validity check
     *    is not passed.
     */
    const std::string &         getRestrictionViolationText  () const;
    
    // ---------------------------------------------------------------------- //
    // Setters
    
    /**
     * @brief resets the Parrot::Restriction object to a state as if it was just
     *    created using the empty Constructor.
     */
    void reset();
    // ...................................................................... //

    /**
     * @brief removes the restriction to be applied before parsing the keyword
     *    line.
     *
     * Sets the \c preParseRestrictionType to \c RestrictionType::None and
     *    removes the associated specifics to the previously stored restriction.
     */
    void resetPreParseRestriction();
    // ...................................................................... //

    /**
     * @brief removes the restriction to be applied after parsing the keyword
     *    line.
     *
     * Sets the \c aftParseRestrictionType to \c RestrictionType::None and
     *    removes the associated specifics to the previously stored restriction.
     */
    void resetAftParseRestriction();
    // ...................................................................... //

    /**
     * @brief sets the part of the restriction applied after parsing the line to
     *    the indicated \c Parrot::ValueType() such that it represents a
     *    \c Parrot::RestrictionType::Range() (cf. \c Parrot::RestrictionType())
     *
     * @param min the least value to be accepted as valid
     * @param max the largest value to be accepted as valid
     *
     * @attention *Parrot* does not check whether <tt>min < max</tt> actually
     *    holds
     *
     */
    void setAftParseRange(const double min, const double max);
    // ...................................................................... //
    
    /**
     * @brief sets the part of the restriction applied before parsing the line
     *    such that it represents a \c Parrot::RestrictionType::AllowedList or
     *    a \c Parrot::RestrictionType::ForbiddenList, respectively.
     *
     * @param list the list of allowed or forbidden values
     * @param forbiddenList if set to true, the installed restriction will be
     *    a \c Parrot::RestrictionType::ForbiddenList; otherwise, it will be a
     *    \c Parrot::RestrictionType::AllowedList.
     */
    void setPreParseValidationList(const std::vector<std::string> & list, bool forbiddenList = false);
    // ...................................................................... //

    /**
     * @brief sets the part of the restriction applied after parsing the line to
     *    the indicated \c Parrot::ValueType() such that it represents a
     *    \c Parrot::RestrictionType::AllowedList or a
     *    \c Parrot::RestrictionType::ForbiddenList, respectively.
     *
     * @param list the list of allowed or forbidden values
     * @param forbiddenList if set to true, the installed restriction will be
     *    a \c Parrot::RestrictionType::ForbiddenList; otherwise, it will be a
     *    \c Parrot::RestrictionType::AllowedList.
     */
    template<typename T>
    void setAftParseValidationList(const std::vector<T>           & list, bool forbiddenList = false);
    // ...................................................................... //
    
    /**
     * @brief sets the part of the restriction applied before parsing the line
     *    such that it represents a \c Parrot::RestrictionType::Function.
     *
     * @param uFunc the function to be called to decide whether or not a value
     *    is valid.
     */
    void setPreParseValidationFunction(const std::function<bool (const std::string &)> & uFunc);
    // ...................................................................... //

    /**
     * @brief sets the part of the restriction applied after parsing the line to
     *    the indicated \c Parrot::ValueType() such that it represents a
     *    \c Parrot::RestrictionType::Function.
     *
     * @param uFunc the function to be called to decide whether or not a value
     *    is valid.
     */
    template<typename T>
    void setAftParseValidationFunction(const std::function<bool (const T &)> &           uFunc);
    // ...................................................................... //
    
    /**
     * @brief specifies the behaviour when a read value does not meet the
     *    critera given by the Parrot::Restriction.
     */
    void setRestrictionViolationPolicy (RestrictionViolationPolicy restrictionViolationPolicy, const std::string & restrictionViolationText);
    // ...................................................................... //
    /**
     * @brief Shortcut to
     * <tt>setRestrictionViolationPolicy(RestrictionViolationPolicy::Warning  , text);</tt>
     */
    void setViolationWarningText  (const std::string & restrictionViolationText);
    // ...................................................................... //
    /**
     * @brief Shortcut to
     * <tt>setRestrictionViolationPolicy(RestrictionViolationPolicy::Exception, text);</tt>
     */
    void setViolationExceptionText(const std::string & restrictionViolationText);
    
    // ---------------------------------------------------------------------- //
    // Representation
    
    /**
     * @brief returns a
     */
    std::string to_string() const;
  };
}

// ========================================================================== //
// template implementations

#include "Parrot/Restriction.tpp"

// ========================================================================== //
#endif
