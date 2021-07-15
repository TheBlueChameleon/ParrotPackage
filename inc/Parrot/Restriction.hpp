/* TODO: File descrption
 * 
 */

#ifndef PARROT_RESTRICTION_HPP
#define PARROT_RESTRICTION_HPP

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
   * the line from the .ini file into its designated \c Parrot::ValueTypeID().
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

  //! @todo Restriction: make a CTor with a string argument, creating a valid characters check function


  class Restriction {
  private:
    RestrictionType preParseRestrictionType = RestrictionType::None;
    std::any        preParseRestriction;
    
    RestrictionType aftParseRestrictionType = RestrictionType::None;
    std::any        aftParseRestriction;

    RestrictionValueTypeID      restrictionValueTypeID     = RestrictionValueTypeID::None;
    
    RestrictionViolationPolicy  restrictionViolationPolicy = RestrictionViolationPolicy::Exception;
    std::string                 restrictionViolationText   = "invalid line:\n$L";
    
    // ---------------------------------------------------------------------- //
    // Rectifyers

    void rectify_AftParseValidationList ();

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
    Restriction(RestrictionViolationPolicy restrictionViolationPolicy,
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
    Restriction(PARROT_TYPE(ValueTypeID::Real) min,
                PARROT_TYPE(ValueTypeID::Real) max,
                RestrictionViolationPolicy     restrictionViolationPolicy = RestrictionViolationPolicy::Exception,
                const std::string &            restrictionViolationText = "value out of bounds"
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
    Restriction(const PARROT_TYPE(ValueTypeID::StringList) & list,
                bool                                         forbiddenList = false,
                RestrictionViolationPolicy                   restrictionViolationPolicy = RestrictionViolationPolicy::Exception,
                const std::string &                          restrictionViolationText = "value not allowed"
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
    Restriction(const std::function<bool (const PARROT_TYPE(ValueTypeID::String) &)> & uFunc,
                RestrictionViolationPolicy                                             restrictionViolationPolicy = RestrictionViolationPolicy::Exception,
                const std::string &                                                    restrictionViolationText = "value not allowed"
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
     *  <tr><td>\c AllowedList   <td> <tt>PARROT_TYPE(ValueTypeID::StringList)</tt>
     *  <tr><td>\c ForbiddenList <td> <tt>PARROT_TYPE(ValueTypeID::StringList)</tt>
     *  <tr><td>\c Range         <td> <em>(not applicable for
     *                                <tt>preParseRestriction</tt>s)</em>
     *  <tr><td>\c Function      <td> <tt>const std::function&lt;bool (const PARROT_TYPE(ValueTypeID::String) &)&gt; &</tt>
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
     *  <tr><td>\c Range         <td> <tt>std::pair&lt;PARROT_TYPE(ValueTypeID::Real), PARROT_TYPE(ValueTypeID::Real)&gt;</tt>
     *  <tr><td>\c Function      <td> <tt>const std::function&lt;bool (const T &)&gt; &</tt>
     * </table>
     *
     * In this, \c T is any PARROT_TYPE() (See \c Parrot::ValueTypeID()).
     */
    const std::any &  getAftParseRestriction    () const;
    // ...................................................................... //
    
    
    /**
     * @brief returns the boundaries between which a parsed value should be
     *    considered valid as a <tt>std::pair&lt;double, double&gt;</tt>, if the
     *    \c aftParseRestriction is set to \c Parrot::RestrictionType::Range.
     *    Otherwise, an error is thrown.
     *
     * @returns a <tt>std::pair&lt;double, double&gt; = {min, max}</tt>
     *
     * @throws Parrot::RestrictionTypeError if \c aftParseRestriction is not
     *    equal to \c Parrot::RestrictionType::Range.
     */
    const std::pair<PARROT_TYPE(ValueTypeID::Real), PARROT_TYPE(ValueTypeID::Real)> getAftParseRange() const;
    // ...................................................................... //
    
    /**
     * @brief returns the validation list according to which an unparsed line
     *    in the .ini file should be considered valid as a
     *    \c std::vector<std::string>, if the \c aftParseRestriction is set to
     *    \c Parrot::RestrictionType::AllowedList or
     *    \c Parrot::RestrictionType::ForbiddenList, respectively.
     *    Otherwise, an error is thrown.
     *
     * @returns a \c std::vector<std::string> holding all allowed or forbidden
     *    values for the keyword being specified.
     *
     * @throws Parrot::RestrictionTypeError if \c aftParseRestriction is neither
     *    equal to \c Parrot::RestrictionType::AllowedList nor to
     *    \c Parrot::RestrictionType::ForbiddenList.
     */
    const PARROT_TYPE(ValueTypeID::StringList)      getPreParseValidationList() const;
    // ...................................................................... //

    /**
     * @brief returns the validation list according to which an parsed line in
     *    the .ini file should be considered valid as a \c std::vector<T> where
     *    \c T depends on the Parrot::ValueTypeID().
     *
     * @returns a \c std::vector<T> containing all the allowed values for the
     *    keyword (if \c aftParseRestriction is equal to
     *    \c Parrot::RestrictionType::AllowedList) or all the forbidden values
     *    (if \c aftParseRestriction is equal to
     *    \c Parrot::RestrictionType::ForbiddenList), respectively.
     *
     * @throws Parrot::RestrictionTypeError if \c aftParseRestriction is neither
     *    equal to \c Parrot::RestrictionType::AllowedList nor to
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
     *
     * @throws Parrot::RestrictionTypeError if \c aftParseRestriction is not
     *    equal to \c Parrot::RestrictionType::Function.
     */
    const std::function<bool (const PARROT_TYPE(ValueTypeID::String) &)> getPreParseValidationFunction() const;
    // ...................................................................... //

    /**
     * @brief returns a \c std::function object representing the user defined
     *    function to be called to check validity of a keyword after parsing
     *    to the Parrot::ValueTypeID().
     *
     * @returns the \c std::function to be called to check for keyword validity,
     *    if \c preParseRestriction is equal to
     *    \c Parrot::RestrictionType::Function. Otherwise, throws an error.
     *
     * @throws Parrot::RestrictionTypeError if \c aftParseRestriction is not
     *    equal to \c Parrot::RestrictionType::Function.
     */
    template<typename T>
    const std::function<bool (const T &)>           getAftParseValidationFunction () const;
    // ...................................................................... //
    

    /**
     * @brief returns a code that determines which kinds of keywords can be used
     *    with this restriction. See \c RestrictionValueTypeID() for details.
     */
    RestrictionValueTypeID                          getRestrictionValueTypeID() const;
    // ...................................................................... //

    /**
     * @brief returns the Parrot::RestrictionViolationPolicy() applied when the
     *    described validity check is not passed.
     */
    RestrictionViolationPolicy                      getRestrictionViolationPolicy() const;
    // ...................................................................... //

    /**
     * @brief returns the text that is output when the described validity check
     *    is not passed.
     */
    const std::string &                             getRestrictionViolationText  () const;
    
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
     *    the indicated \c Parrot::ValueTypeID() such that it represents a
     *    \c Parrot::RestrictionType::Range() (cf. \c Parrot::RestrictionType())
     *
     * @param min the least value to be accepted as valid
     * @param max the largest value to be accepted as valid
     *
     * @attention *Parrot* does not check whether <tt>min < max</tt> actually
     *    holds
     */
    void setAftParseRange(const PARROT_TYPE(ValueTypeID::Real) min, const PARROT_TYPE(ValueTypeID::Real) max);
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
    void setPreParseValidationList(const PARROT_TYPE(ValueTypeID::StringList) & list, bool forbiddenList = false);
    // ...................................................................... //

    /**
     * @brief sets the part of the restriction applied after parsing the line to
     *    the indicated \c Parrot::ValueTypeID() such that it represents a
     *    \c Parrot::RestrictionType::AllowedList or a
     *    \c Parrot::RestrictionType::ForbiddenList, respectively.
     *
     * @param list the list of allowed or forbidden values
     * @param forbiddenList if set to true, the installed restriction will be
     *    a \c Parrot::RestrictionType::ForbiddenList; otherwise, it will be a
     *    \c Parrot::RestrictionType::AllowedList.
     *
     * @throws Parrot::RestrictionTypeError if T is not a *Parrot* list type or
     *    if it names a boolean list
     * @throws Parrot::ValueTypeError if T cannot be mapped onto a *Parrot* list
     *    type
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
     *
     * @throws Parrot::InvalidFunctionError if the function either points to
     *    NULL or does not comply by the signature requirement.
     */
    void setPreParseValidationFunction(const std::function<bool (const PARROT_TYPE(ValueTypeID::String) &)> & uFunc);
    // ...................................................................... //

    /**
     * @brief sets the part of the restriction applied *after* parsing the line
     *    to the indicated \c Parrot::ValueTypeID() such that it represents a
     *    \c Parrot::RestrictionType::Function.
     *
     * @param uFunc the function to be called to decide whether or not a value
     *    is valid.
     *
     * @throws Parrot::InvalidFunctionError if the function either points to
     *    NULL or does not comply by the signature requirement.
     */
    void setAftParseValidationFunction(const std::function<bool (const PARROT_TYPE(ValueTypeID::String     ) &)> & uFunc);
    //! @overload void setAftParseValidationFunction(const std::function<bool (const PARROT_TYPE(ValueTypeID::Integer    ) &)> & uFunc)
    void setAftParseValidationFunction(const std::function<bool (const PARROT_TYPE(ValueTypeID::Integer    ) &)> & uFunc);
    //! @overload void setAftParseValidationFunction(const std::function<bool (const PARROT_TYPE(ValueTypeID::Real       ) &)> & uFunc)
    void setAftParseValidationFunction(const std::function<bool (const PARROT_TYPE(ValueTypeID::Real       ) &)> & uFunc);
    //! @overload void setAftParseValidationFunction(const std::function<bool (const PARROT_TYPE(ValueTypeID::Boolean    ) &)> & uFunc)
    void setAftParseValidationFunction(const std::function<bool (const PARROT_TYPE(ValueTypeID::Boolean    ) &)> & uFunc);
    //! @overload void setAftParseValidationFunction(const std::function<bool (const PARROT_TYPE(ValueTypeID::StringList ) &)> & uFunc)
    void setAftParseValidationFunction(const std::function<bool (const PARROT_TYPE(ValueTypeID::StringList ) &)> & uFunc);
    //! @overload void setAftParseValidationFunction(const std::function<bool (const PARROT_TYPE(ValueTypeID::IntegerList) &)> & uFunc)
    void setAftParseValidationFunction(const std::function<bool (const PARROT_TYPE(ValueTypeID::IntegerList) &)> & uFunc);
    //! @overload void setAftParseValidationFunction(const std::function<bool (const PARROT_TYPE(ValueTypeID::RealList   ) &)> & uFunc)
    void setAftParseValidationFunction(const std::function<bool (const PARROT_TYPE(ValueTypeID::RealList   ) &)> & uFunc);
    //! @overload void setAftParseValidationFunction(const std::function<bool (const PARROT_TYPE(ValueTypeID::BooleanList) &)> & uFunc)
    void setAftParseValidationFunction(const std::function<bool (const PARROT_TYPE(ValueTypeID::BooleanList) &)> & uFunc);
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
    void setViolationWarningText      (const std::string & restrictionViolationText);
    // ...................................................................... //
    /**
     * @brief Shortcut to
     * <tt>setRestrictionViolationPolicy(RestrictionViolationPolicy::WarningRevert, text);</tt>
     */
    void setViolationWarningRevertText(const std::string & restrictionViolationText);
    // ...................................................................... //
    /**
     * @brief Shortcut to
     * <tt>setRestrictionViolationPolicy(RestrictionViolationPolicy::Exception, text);</tt>
     */
    void setViolationExceptionText    (const std::string & restrictionViolationText);
    // ...................................................................... //
    
    // ---------------------------------------------------------------------- //
    // Representation
    
    /**
     * @brief returns a text rendition of the Parrot::Restriction for debug
     *    purposes
     *
     * Example 1:
     * @code
     * Parrot::Restriction r;
     * std::cout << r.to_string() << std::endl;
     * @endcode
     *
     * Output:
     @verbatim
     Restriction
       Pre-Parsing Restriction: none
       Post-Parsing Restriction: none
       Violation Policy: throw a RestrictionViolationError
         Message: invalid line
     @endverbatim
     *
     * Example 2:
     * @code
     * Parrot::Restriction r;
     * r.setPreParseValidationList({"A", "B", "Cebra"});
     * r.setRestrictionViolationPolicy(Parrot::RestrictionViolationPolicy::Warning, "warning text");
     * r.setAftParseValidationList<std::string>({"C", "b", "Aebra"}, true);
     * std::cout << r.to_string() << std::endl;
     * @endcode
     *
     * Output:
     @verbatim
     Restriction
       Pre-Parsing Restriction: list of allowed values
         List: [A,B,Cebra]
       Post-Parsing Restriction: list of forbidden values
         List: (### given ###)
       Violation Policy: utter a warning via stderr
         Message: warning text
     @endverbatim
     *
     */
    std::string to_string() const;
  };
}

// ========================================================================== //
// template implementations

#include "Parrot/Restriction.tpp"

// ========================================================================== //
#endif
