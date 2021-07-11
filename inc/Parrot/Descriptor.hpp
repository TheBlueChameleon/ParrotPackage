/* TODO: File descrption
 * 
 */

#ifndef PARROT_DESCRIPTOR_HPP
#define PARROT_DESCRIPTOR_HPP

// ========================================================================== //
// dependencies

// STL
#include <string>
#include <vector>
#include <tuple>
#include <any>
#include <initializer_list>

#include <functional>

// own
#include "Parrot/Definitions.hpp"
#include "Parrot/Restriction.hpp"

// ========================================================================== //

namespace Parrot {

  // ======================================================================== //
  // class

  /**
   * @brief Describes a single line in a ini file, complete with parsing rules
   * and limits.
   *
   * The below parameters have the same meaning across all class members.
   *
   * @param key the keyword in the ini file to which this Descriptor pertains
   *
   * @param valueType a data type index that describes into which format the
   *    text from the ini file is parsed.
   *
   * @param defaultValue the value to which a keyword will default if not given
   *    in the ini file. If set, by default the mandatory parameter will be set
   *    to false.<br>
   *    Supported are ```std::string```, ```int```, ```double```, ```bool``` and
   *    ```std::vector```s thereof. For convenience, ```std::initializer_list```s
   *    are automatically converted into their ```std::vector``` counterparts.
   *
   * @param policy the effect that is triggered when a read keyword does not
   *    fall within the specified range. See
   *    \c Parrot::RestrictionViolationPolicy() for details
   *
   * @param restrictionViolationText the text to be prompted with the event
   *    triggered when a read value does not fall within the specified range.

   * @param mandatory whether an ini file missing this keyword should be
   *    considered incomplete (triggers a warning or an exception)
   *
   */
  class Descriptor {
  private:
    std::string   key;
    std::any      value;                                                        // default value, if 'key' is not in file
    ValueTypeID   valueTypeID = ValueTypeID::Integer;
    
    bool          keyCaseSensitive        = false;
    bool          valueCaseSensitive      = false;
    
    bool          trimLeadingWhitespaces  = true;                               // only affects values, not keys
    bool          trimTrailingWhitespaces = true;
    
    bool          mandatory               = false;

    char          listSeparator           = ',';                                // ignored for non-list types
    
    std::vector<Restriction>                          restrictions;
    
    // defines a dictionary for the first step of the parsing process
    std::vector<std::pair<PARROT_TYPE(ValueTypeID::String), PARROT_TYPE(ValueTypeID::String)>>  substitutions;

    // will be called after doing the substitutions with the read value as an argument. Output is user-parsed line
    std::function<PARROT_TYPE(ValueTypeID::String) (const PARROT_TYPE(ValueTypeID::String) &)>  userPreParser;
    
    // ---------------------------------------------------------------------- //
    // Rectifyers

    void rectify();                                                             // make sure an arbitrary input type gets mapped to the corresponding Parrot type.
    void rectify_String     ();
    void rectify_Integer    ();
    void rectify_Real       ();
    void rectify_Boolean    ();
    void rectify_StringList ();
    void rectify_IntegerList();
    void rectify_RealList   ();
    void rectify_BooleanList();

  public:
    // ---------------------------------------------------------------------- //
    // CTors
    
    //! Creates an empty Descriptor element
    Descriptor() = default;

    //! Creates a Descriptor element with a key and an associated data type and no Restrictions
    Descriptor(std::string key, ValueTypeID valueType = ValueTypeID::Integer , bool mandatory = true);

    //! Creates a Descriptor element with a key and an associated default value (data type derived thereof) and no Restrictions
    template <typename T>
    Descriptor(std::string key, const T                        & defaultValue, bool mandatory = false);

    //! Creates a Descriptor element with a key and an associated default value (data type derived thereof) for a list with no Restrictions
    template <typename T>
    Descriptor(std::string key, const std::initializer_list<T> & defaultValue, bool mandatory = false);

    // ---------------------------------------------------------------------- //
    // Getters
    
    //! returns the name of the keyword
    const std::string getKey          () const;
    //! returns the default value for the keyword
    std::any          getValue        () const;
    //! returns the \c ValueTypeID() of the keyword
    ValueTypeID       getValueTypeID  () const;
    //! returns a human readable form of the \c ValueTypeID() of the keyword
    const std::string getValueTypeName() const;

    /*! @brief returns a C++ style interpretation of the data type of the
     *    keyword. Same as <tt>std::string(getValue().type().name())</tt>
     *
     * The data type \c std::any keeps track of its current content by means of
     * a \c std::type_index member that can be read using the \c type() method
     * and rendered into a string using the \c name() method. These identifiers,
     * while unambiguous are barely legible (when compiled with the GCC or
     * clang) and should be mostly used for debug.
     *
     * To get a human readable typre representation, use
     *  \c Parrot::getValueTypeName() for an Parrot-internal type name or
     *  \c getTypeIDDemangled() for the C++ full type name.
     */
    const std::string getTypeID            () const;
    //! @brief returns the C++ type name of the keyword
    const std::string getTypeIDDemangled   () const;
    
    //! returns \c true if the keyword is to be treated case sensitively
    bool          isKeyCaseSensitive       () const;
    //! returns \c true if the value is to be treated case sensitively
    bool          isValueCaseSensitive     () const;
    
    //! returns \c true if leading whitespaces are to be removed before parsing
    bool          isTrimLeadingWhitespaces () const;
    //! returns \c true if trailing whitespaces are to be removed before parsing
    bool          isTrimTrailingWhitespaces() const;
    
    //! returns \c true if an error should be thrown if the keyword was not found in the .ini file
    bool          isMandatory              () const;

    //! returns the character used to separate list elemetns (ignored for non-list types)
    char          getListSeparator         () const;
    
    //! returns the collection of all <tt>Parrot::Restriction</tt>s applied to the keyword
    const std::vector<Restriction>                                                                   & getRestrictions () const;

    /**
     * @brief returns a list of all string-substitions to be made in the parsing
     *    process.
     *
     * The returned object is a <tt>std::vector&lt;std::pair&gt;&gt;</tt>, which
     *    means that the vector elements have members \c first and \c second.
     *    Of these, \c first represents the term to replace while \c second is
     *    the value it will be replaced with.
     */
    const std::vector<std::pair<PARROT_TYPE(ValueTypeID::String), PARROT_TYPE(ValueTypeID::String)>> & getSubstitutions() const;
    //! returns the function to be called at the beginning of the string parsing stage
    const std::function<PARROT_TYPE(ValueTypeID::String) (const PARROT_TYPE(ValueTypeID::String) &)> & getUserPreParser() const;
    
    // ---------------------------------------------------------------------- //
    // Setters
    
    //! restores the state as if initialized by the empty CTor
    void reset ();
    //! sets the keyword to emtpy
    void resetKey();
    //! removes the default value and value type
    void resetValue();
    //! restores the state as if initialized by the empty CTor but does not affect keyword name, value type or default value
    void resetMetaData();

    //! sets the keyword name
    void setKey (const std::string & newVal);

    /**
     * @brief sets a new default value
     *
     * @param newVal the new default value
     * @param resetMetaData if true, resetMetaData() is called before applying
     *    changes
     *
     * This will also set the \c Parrot::ValueTypeID() according to the type of
     *    \c newVal. See \c Parrot::valueTypeIDOf() for deduction rules.
     *
     */
    template<typename T>
    void setValue(const T & newVal, bool resetMetaData = true);

    //! @overload void setValue(const std::initializer_list<T> & list, bool resetMetaData = true)
    template<typename T>
    void setValue(const std::initializer_list<T> & list, bool resetMetaData = true);

    // MetaData
    //! sets whether or not the keyword name is to be read case sensitively
    void setKeyCaseSensitive        (bool newVal);
    //! sets whether or not the keyword value is to be read case sensitively
    void setValueCaseSensitive      (bool newVal);

    //! sets whether or not to remove leading withe spaces from the keyword value
    void setTrimLeadingWhitespaces  (bool newVal);
    //! sets whether or not to remove trailing withe spaces from the keyword value
    void setTrimTrailingWhitespaces (bool newVal);

    //! sets whether or not an error should be thrown if the keyword was not found in the .ini file
    void setMandatory               (bool newVal);
    
    //! sets the character used to separate list elemetns (ignored for non-list types)
    void setListSeparator           (const char newVal);


    /**
     * @brief adds a \c Parrot::Restriction to the described keyword
     *
     * @param newVal a new \c Parrot::Restriction to be added to the list of
     *    restrictions applied to the keyword value when read from file.
     *    The \c Parrot::Restriction will be copied from the source. That is,
     *    no reference is stored and the original object may be destroyed or
     *    changed after calling this method.
     *
     * @note A \c Parrot::Restriction may not be compatible with the valueType
     *    of a keywor value. For example, a \c Parrot::RestrictionType::Range is
     *    incompatible with a \c Parrot::ValueTypeID::String.<br>
     *    See \c Parrot::RestrictionValueTypeID() for details.
     *
     * @throws std::runtime_error if \c newVal is not compatible with the
     *    valueType of the keyword
     */
    void addRestriction  (const Restriction & newVal);
    //! removes all restrictions attached to a keyword
    void clearRestrictions();
    
    /**
     * @brief adds a string-substitions to be made in the parsing process.
     *
     * @param substituee a substring to be searched for in the keyword value
     *    before converting it to the target value type
     * @param substitute the string to replace the \c substituee with
     *
     * Substitutions are made *after* the userParser function is applied. See
     * also \c setUserPreParser() for details.
     */
    void addSubstitution (const PARROT_TYPE(ValueTypeID::String) & substituee, const PARROT_TYPE(ValueTypeID::String) & substitute);
    //! removes all substitions applied to a keyword
    void clearSubstitutions ();
    
    /**
     * @brief sets a function that is applied as a first step of the parsing
     *    process.
     *
     * That is, *Parrot* applies<br>
     *    <tt>value = userPreParser(value)</tt><br>
     *    before doing the substitutions (cf. \c addSubstitution() and finally
     *    converting to the target value type.
     */
    void setUserPreParser(const std::function<PARROT_TYPE(ValueTypeID::String) (const PARROT_TYPE(ValueTypeID::String) &)> & newVal);
    //! removes a set user preparser. (cf \c setUserPreParser().)
    void clearUserPreParser();
    

    /**
     * @brief creates a descriptor for a keyword with no default value and a
     *    range of allowed values
     *
     * @param min the smallest allowed value
     * @param max the largest allowed value
     *
     * @attention it is not checked whether or not \c min &lt; \c max!
     *
     * @throws std::runtime_error according to \c addRestriction().
     */
    void makeRanged(const std::string &                                          key,
                    ValueTypeID                                                  valueType,
                    PARROT_TYPE(ValueTypeID::Real)                               min,
                    PARROT_TYPE(ValueTypeID::Real)                               max,
                    RestrictionViolationPolicy                                   policy = RestrictionViolationPolicy::Exception,
                    const std::string &                                          restrictionViolationText = "value out of bounds",
                    bool                                                         mandatory = true
    );

    /**
     * @brief creates a descriptor for a keyword with a default value and a
     *    range of allowed values
     *
     * @param min the smallest allowed value
     * @param max the largest allowed value
     *
     * @attention it is not checked whether or not \c min &lt; \c max!
     *
     * @throws std::runtime_error according to \c addRestriction().
     */
    template <typename T>
    void makeRanged(const std::string &                                          key,
                    const T &                                                    defaultValue,
                    PARROT_TYPE(ValueTypeID::Real)                               min,
                    PARROT_TYPE(ValueTypeID::Real)                               max,
                    RestrictionViolationPolicy                                   policy = RestrictionViolationPolicy::Exception,
                    const std::string &                                          restrictionViolationText = "value out of bounds",
                    bool                                                         mandatory = false
    );


    /**
     * @brief creates a descriptor for a keyword with no default value and a
     *    list of allowed or forbidden values, respectively, that is applied
     *    before parsing begins (and in particular before the user preparser is
     *    applied)
     *
     * @param list the list to check the keyword value against
     * @param forbiddenlist if set to \c true, \c list specifies the only
     *    allowed keyword values. Otherwise, it specifies values that may not be
     *    used.
     *
     * @throws std::runtime_error according to \c addRestriction().
     */
    void makeListboundPreParse(const std::string &                               key,
                               ValueTypeID                                       valueType,
                               const PARROT_TYPE(ValueTypeID::StringList) &      list,
                               bool                                              forbiddenList = false,
                               RestrictionViolationPolicy                        policy = RestrictionViolationPolicy::Exception,
                               const std::string &                               restrictionViolationText = "value out of bounds",
                               bool                                              mandatory = true
    );

    /**
     * @brief creates a descriptor for a keyword with a default value and a
     *    list of allowed or forbidden values, respectively, that is applied
     *    before parsing begins (and in particular before the user preparser is
     *    applied)
     *
     * @param list the list to check the keyword value against
     * @param forbiddenlist if set to \c true, \c list specifies the only
     *    allowed keyword values. Otherwise, it specifies values that may not be
     *    used.
     *
     * @throws std::runtime_error according to \c addRestriction().
     */
    template <typename T>
    void makeListboundPreParse(const std::string &                               key,
                               const T &                                         defaultValue,
                               const PARROT_TYPE(ValueTypeID::StringList) &      list,
                               bool                                              forbiddenList = false,
                               RestrictionViolationPolicy                        policy = RestrictionViolationPolicy::Exception,
                               const std::string &                               restrictionViolationText = "value out of bounds",
                               bool                                              mandatory = false
    );


    /**
     * @brief creates a descriptor for a keyword with no default value and a
     *    list of allowed or forbidden values, respectively, that is applied
     *    after parsing the keyword value to the target value type.
     *
     * @param list the list to check the keyword value against
     * @param forbiddenlist if set to \c true, \c list specifies the only
     *    allowed keyword values. Otherwise, it specifies values that may not be
     *    used.
     *
     * @throws std::runtime_error according to \c addRestriction().
     */
    template <typename LT>
    void makeListboundAftParse(const std::string &                               key,
                               ValueTypeID                                       valueType,
                               const std::vector<LT> &                           list,
                               bool                                              forbiddenList = false,
                               RestrictionViolationPolicy                        policy = RestrictionViolationPolicy::Exception,
                               const std::string &                               restrictionViolationText = "value out of bounds",
                               bool                                              mandatory = true
    );

    /**
     * @brief creates a descriptor for a keyword with a default value and a
     *    list of allowed or forbidden values, respectively, that is applied
     *    after parsing the keyword value to the target value type.
     *
     * @param list the list to check the keyword value against
     * @param forbiddenlist if set to \c true, \c list specifies the only
     *    allowed keyword values. Otherwise, it specifies values that may not be
     *    used.
     *
     * @throws std::runtime_error according to \c addRestriction().
     */
    template <typename DT, typename LT>
    void makeListboundAftParse(const std::string &                               key,
                               const DT &                                        defaultValue,
                               const std::vector<LT> &                           list,
                               bool                                              forbiddenList = false,
                               RestrictionViolationPolicy                        policy = RestrictionViolationPolicy::Exception,
                               const std::string &                               restrictionViolationText = "value out of bounds",
                               bool                                              mandatory = false
    );
    

    /**
     * @brief creates a descriptor for a keyword with no default value that is
     *    checked against a user defined function for validity before parsing
     *    begins.
     *
     * @param uFunc the function that assesses whether a keyword value is deemed
     *    valid. That is, <tt>uFunc(value)</tt> needs to be \c true for the
     *    keyword value to be accepted
     *
     * @throws std::runtime_error according to \c addRestriction().
     */
    void makeUserboundPreParse(const std::string &                               key,
                               ValueTypeID                                       valueType,
                               const std::function<bool (const std::string &)> & uFunc,
                               RestrictionViolationPolicy                        policy = RestrictionViolationPolicy::Exception,
                               const std::string &                               restrictionViolationText = "value out of bounds",
                               bool                                              mandatory = true
    );
    
    /**
     * @brief creates a descriptor for a keyword with a default value that is
     *    checked against a user defined function for validity before parsing
     *    begins.
     *
     * @param uFunc the function that assesses whether a keyword value is deemed
     *    valid. That is, <tt>uFunc(value)</tt> needs to be \c true for the
     *    keyword value to be accepted
     *
     * @throws std::runtime_error according to \c addRestriction().
     */
    template <typename T>
    void makeUserboundPreParse(const std::string &                               key,
                               const T &                                         defaultValue,
                               const std::function<bool (const PARROT_TYPE(ValueTypeID::String) &)> & uFunc,
                               RestrictionViolationPolicy                        policy = RestrictionViolationPolicy::Exception,
                               const std::string &                               restrictionViolationText = "value out of bounds",
                               bool                                              mandatory = false
    );


    /**
     * @brief creates a descriptor for a keyword with no default value that is
     *    checked against a user defined function for validity after parsing
     *    to the value type.
     *
     * @param uFunc the function that assesses whether a keyword value is deemed
     *    valid. That is, <tt>uFunc(value)</tt> needs to be \c true for the
     *    keyword value to be accepted
     *
     * @throws std::runtime_error according to \c addRestriction().
     */
    template <typename T>
    void makeUserboundAftParse(const std::string &                               key,
                               ValueTypeID                                       valueType,
                               const std::function<bool (const T &)> &           uFunc,
                               RestrictionViolationPolicy                        policy = RestrictionViolationPolicy::Exception,
                               const std::string &                               restrictionViolationText = "value out of bounds",
                               bool                                              mandatory = true
    );

    /**
     * @brief creates a descriptor for a keyword with a default value that is
     *    checked against a user defined function for validity after parsing
     *    to the target value type.
     *
     * @param uFunc the function that assesses whether a keyword value is deemed
     *    valid. That is, <tt>uFunc(value)</tt> needs to be \c true for the
     *    keyword value to be accepted
     *
     * @throws std::runtime_error according to \c addRestriction().
     */
    template <typename T>
    void makeUserboundAftParse(const std::string &                               key,
                               const T &                                         defaultValue,
                               const std::function<bool (const T &)> &           uFunc,
                               RestrictionViolationPolicy                        policy = RestrictionViolationPolicy::Exception,
                               const std::string &                               restrictionViolationText = "value out of bounds",
                               bool                                              mandatory = false
    );
    
    // ---------------------------------------------------------------------- //
    // Representation
    
    /**
     * @brief creates a textual representation of the entire descriptor
     *    including the applied restrictions.
     */
    std::string to_string() const;
  };
}

// ========================================================================== //
// template implementations

#include "Parrot/Descriptor.tpp"

// ========================================================================== //

#endif
