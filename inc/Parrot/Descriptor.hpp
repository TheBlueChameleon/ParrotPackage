/* TODO: File descrption
 * 
 */

#ifndef SETTINGS_DESCRIPTOR_HPP
#define SETTINGS_DESCRIPTOR_HPP

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
    
    const std::string getKey          () const;
    std::any          getValue        () const;
    ValueTypeID       getValueTypeID  () const;
    const std::string getValueTypeName() const;

    /*! @brief returns a C++ style interpretation of the data type of the
     *    keyword. Same as getValue().type().name()
     *
     * The data type \c std::any keeps track of its current content by means of
     * a \c std::type_index member that can be read using the \c type() method
     * and rendered into a string using the \c name() method. These identifiers,
     * while unambiguous are barely legible and should be mostly used for debug.
     *
     * Use the \c Parrot::getValueTypeID() function together with
     * \c Parrot::valueTypeName to get a more human-friendly text representation
     * of the contained data.
     */
    const std::string getTypeID            () const;
    const std::string getTypeIDDemangled   () const;
    
    bool          isKeyCaseSensitive       () const;
    bool          isValueCaseSensitive     () const;
    
    bool          isTrimLeadingWhitespaces () const;
    bool          isTrimTrailingWhitespaces() const;
    
    bool          isMandatory              () const;
    
    const std::vector<Restriction>                                                                   & getRestrictions () const;

    const std::vector<std::pair<PARROT_TYPE(ValueTypeID::String), PARROT_TYPE(ValueTypeID::String)>> & getSubstitutions() const;
    const std::function<PARROT_TYPE(ValueTypeID::String) (const PARROT_TYPE(ValueTypeID::String) &)> & getUserPreParser() const;
    
    // ---------------------------------------------------------------------- //
    // Setters
    
    void reset ();
    void resetKey();
    void resetValue();
    void resetMetaData();

    void setKey (const std::string & newVal);

    template<typename T>
    void setValue(const T & newVal, bool resetMetaData = true);

    template<typename T>
    void setValue(const std::initializer_list<T> & list, bool resetMetaData = true);

    // MetaData
    void setKeyCaseSensitive        (bool newVal);
    void setValueCaseSensitive      (bool newVal);

    void setTrimLeadingWhitespaces  (bool newVal);
    void setTrimTrailingWhitespaces (bool newVal);

    void setMandatory               (bool newVal);
    

    /**
     * @brief adds a \c Parrot::Restriction to the described keyword
     *
     * @throws std::runtime_error if \c newVal is not compatible with the
     *    valueType of the keyword
     */
    void addRestriction  (const Restriction & newVal);
    void clearRestrictions();
    
    void addSubstitution (const PARROT_TYPE(ValueTypeID::String) & substituee, const PARROT_TYPE(ValueTypeID::String) & substitute);
    void clearSubstitutions ();
    
    void setUserPreParser(const std::function<PARROT_TYPE(ValueTypeID::String) (const PARROT_TYPE(ValueTypeID::String) &)> & newVal);
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
     * @throws std::runtime_error if \c valueType is not a number type (i.e.
     *    \c ValueTypeID::Integer, \c ValueTypeID::Real,
     *    \c ValueTypeID::IntegerList or \c ValueTypeID::RealList).
     */
    void makeRanged(const std::string &                                          key,
                    ValueTypeID                                                  valueType,
                    PARROT_TYPE(ValueTypeID::Real)                               min,
                    PARROT_TYPE(ValueTypeID::Real)                               max,
                    RestrictionViolationPolicy                                   policy = RestrictionViolationPolicy::Exception,
                    const std::string &                                          restrictionViolationText = "value out of bounds",
                    bool                                                         mandatory = true
    );

    template <typename T>
    void makeRanged(const std::string &                                          key,
                    const T &                                                    defaultValue,
                    PARROT_TYPE(ValueTypeID::Real)                               min,
                    PARROT_TYPE(ValueTypeID::Real)                               max,
                    RestrictionViolationPolicy                                   policy = RestrictionViolationPolicy::Exception,
                    const std::string &                                          restrictionViolationText = "value out of bounds",
                    bool                                                         mandatory = false
    );


    void makeListboundPreParse(const std::string &                               key,
                               ValueTypeID                                       valueType,
                               const PARROT_TYPE(ValueTypeID::StringList) &      list,
                               bool                                              forbiddenList = false,
                               RestrictionViolationPolicy                        policy = RestrictionViolationPolicy::Exception,
                               const std::string &                               restrictionViolationText = "value out of bounds",
                               bool                                              mandatory = true
    );

    template <typename T>
    void makeListboundPreParse(const std::string &                               key,
                               const T &                                         defaultValue,
                               const PARROT_TYPE(ValueTypeID::StringList) &      list,
                               bool                                              forbiddenList = false,
                               RestrictionViolationPolicy                        policy = RestrictionViolationPolicy::Exception,
                               const std::string &                               restrictionViolationText = "value out of bounds",
                               bool                                              mandatory = false
    );


    template <typename LT>
    void makeListboundAftParse(const std::string &                               key,
                               ValueTypeID                                       valueType,
                               const std::vector<LT> &                           list,
                               bool                                              forbiddenList = false,
                               RestrictionViolationPolicy                        policy = RestrictionViolationPolicy::Exception,
                               const std::string &                               restrictionViolationText = "value out of bounds",
                               bool                                              mandatory = true
    );

    template <typename DT, typename LT>
    void makeListboundAftParse(const std::string &                               key,
                               const DT &                                        defaultValue,
                               const std::vector<LT> &                           list,
                               bool                                              forbiddenList = false,
                               RestrictionViolationPolicy                        policy = RestrictionViolationPolicy::Exception,
                               const std::string &                               restrictionViolationText = "value out of bounds",
                               bool                                              mandatory = false
    );
    

    void makeUserboundPreParse(
      const std::string &                                                        key,
      ValueTypeID                                                                valueType,
      const std::function<bool (const std::string &)> &                          uFunc,
      RestrictionViolationPolicy                                                 policy = RestrictionViolationPolicy::Exception,
      const std::string &                                                        restrictionViolationText = "value out of bounds",
      bool                                                                       mandatory = true
    );
    
    template <typename T>
    void makeUserboundPreParse(const std::string &                               key,
                               const T &                                         defaultValue,
                               const std::function<bool (const PARROT_TYPE(ValueTypeID::String) &)> & uFunc,
                               RestrictionViolationPolicy                        policy = RestrictionViolationPolicy::Exception,
                               const std::string &                               restrictionViolationText = "value out of bounds",
                               bool                                              mandatory = false
    );


    template <typename T>
    void makeUserboundAftParse(const std::string &                               key,
                               ValueTypeID                                       valueType,
                               const std::function<bool (const T &)> &           uFunc,
                               RestrictionViolationPolicy                        policy = RestrictionViolationPolicy::Exception,
                               const std::string &                               restrictionViolationText = "value out of bounds",
                               bool                                              mandatory = true
    );

    template <typename T>
    void makeUserboundAftParse(
      const std::string &                               key,
      const T &                                         defaultValue,
      const std::function<bool (const T &)> &           uFunc,
      RestrictionViolationPolicy                        policy = RestrictionViolationPolicy::Exception,
      const std::string &                               restrictionViolationText = "value out of bounds",
      bool                                              mandatory = false
    );
    
    // ---------------------------------------------------------------------- //
    // Representation
    
    std::string to_string() const;
  };
}

// ========================================================================== //
// template implementations

#include "Parrot/Descriptor.tpp"

// ========================================================================== //

#endif
