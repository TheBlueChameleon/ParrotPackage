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
   * @param mandatory whether an ini file missing this keyword should be
   *    considered incomplete (triggers a warning or an exception)
   *
   */
  class Descriptor {
  private:
    std::string   key;
    std::any      value;                                                        // default value, if 'key' is not in file
    ValueType     valueType = ValueType::Integer;
    
    bool          keyCaseSensitive        = false;
    bool          valueCaseSensitive      = false;
    
    bool          trimLeadingWhitespaces  = true;                               // only affects values, not keys
    bool          trimTrailingWhitespaces = true;
    
    bool          mandatory               = false;
    
    std::vector<Restriction>                                  restrictions;
    
    std::vector<std::pair<std::string, std::string>>          substitutions;    // defines a dictionary for the first step of the parsing process
    std::function<const std::string & (const std::string &)>  userPreParser;    // will be called after doing the substitutions with the read value as an argument. Output is user-parsed line
    
    // ---------------------------------------------------------------------- //
    // Rectifyers

    void rectifyText();                                                         // make sure that (const) char * gets stored as std::string

  public:
    // ---------------------------------------------------------------------- //
    // CTors
    
    //! Creates an empty Descriptor element
    Descriptor() = default;

    //! Creates a Descriptor element with a key and an associated data type and no Restrictions
    Descriptor(std::string key, ValueType valueType = ValueType::Integer     , bool mandatory = true);

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
    ValueType         getValueType    () const;
    const std::string getValueTypeName() const;
    const std::string getTypeID       () const;
    
    bool          isKeyCaseSensitive       () const;
    bool          isValueCaseSensitive     () const;
    
    bool          isTrimLeadingWhitespaces () const;
    bool          isTrimTrailingWhitespaces() const;
    
    bool          isMandatory              () const;
    
    const std::vector<Restriction>                                  & getRestrictions() const;

    const std::vector<std::pair<std::string, std::string>>          & getSubstitutions() const;
    const std::function<const std::string & (const std::string &)>  & getUserPreParser() const;
    
    // ---------------------------------------------------------------------- //
    // Setters
    
    void reset ();
    
    void setKey (const std::string & newVal);
    
    template<typename T>
    void setValue(const T & newVal, bool resetMetaData = true);
    
    template<typename T>
    void setValue(const std::initializer_list<T> & list, bool resetMetaData = true);
    
    //! @todo implement Parrot::Descriptor::resetValue
    void resetValue();

    void setKeyCaseSensitive        (bool newVal);
    void setValueCaseSensitive      (bool newVal);

    void setTrimLeadingWhitespaces  (bool newVal);
    void setTrimTrailingWhitespaces (bool newVal);

    void setMandatory               (bool newVal);
    

    //! @todo add test whether newVal is a valid restriction for that valueType
    void addRestriction  (const Restriction & newVal);
    void clearRestrictions();
    
    void addSubstitution (const std::string & substituee, const std::string & substitute);
    void clearSubstitutions ();
    
    void setUserPreParser(const std::function<const std::string & (const std::string &)> & newVal);
    void clearUserPreParser();
    

    void makeRanged(
      const std::string &                               key,
      ValueType                                         valueType,
      double min, double max,
      RestrictionViolationPolicy                        policy = RestrictionViolationPolicy::Exception,
      const std::string &                               restrictionViolationText = "value out of bounds",
      bool                                              mandatory = true
    );
    
    template <typename T>
    void makeRanged(
      const std::string &                               key,
      const T &                                         defaultValue,
      double min, double max,
      RestrictionViolationPolicy                        policy = RestrictionViolationPolicy::Exception,
      const std::string &                               restrictionViolationText = "value out of bounds",
      bool                                              mandatory = false
    );


    void makeListboundPreParse(
      const std::string &                               key,
      ValueType                                         valueType,
      const std::vector<std::string> &                  list,
      bool                                              forbiddenList = false,
      RestrictionViolationPolicy                        policy = RestrictionViolationPolicy::Exception,
      const std::string &                               restrictionViolationText = "value out of bounds",
      bool                                              mandatory = true
    );
    
    template <typename T>
    void makeListboundPreParse(
      const std::string &                               key,
      const T &                                         defaultValue,
      const std::vector<std::string> &                  list,
      bool                                              forbiddenList = false,
      RestrictionViolationPolicy                        policy = RestrictionViolationPolicy::Exception,
      const std::string &                               restrictionViolationText = "value out of bounds",
      bool                                              mandatory = false
    );


    template <typename LT>
    void makeListboundAftParse(
      const std::string &                               key,
      ValueType                                         valueType,
      const std::vector<LT> &                           list,
      bool                                              forbiddenList = false,
      RestrictionViolationPolicy                        policy = RestrictionViolationPolicy::Exception,
      const std::string &                               restrictionViolationText = "value out of bounds",
      bool                                              mandatory = true
    );

    template <typename DT, typename LT>
    void makeListboundAftParse(
      const std::string &                               key,
      const DT &                                        defaultValue,
      const std::vector<LT> &                           list,
      bool                                              forbiddenList = false,
      RestrictionViolationPolicy                        policy = RestrictionViolationPolicy::Exception,
      const std::string &                               restrictionViolationText = "value out of bounds",
      bool                                              mandatory = false
    );
    

    void makeUserboundPreParse(
      const std::string &                               key,
      ValueType                                         valueType,
      const std::function<bool (const std::string &)> & uFunc,
      RestrictionViolationPolicy                        policy = RestrictionViolationPolicy::Exception,
      const std::string &                               restrictionViolationText = "value out of bounds",
      bool                                              mandatory = true
    );
    
    template <typename T>
    void makeUserboundPreParse(
      const std::string &                               key,
      const T &                                         defaultValue,
      const std::function<bool (const std::string &)> & uFunc,
      RestrictionViolationPolicy                        policy = RestrictionViolationPolicy::Exception,
      const std::string &                               restrictionViolationText = "value out of bounds",
      bool                                              mandatory = false
    );


    template <typename T>
    void makeUserboundAftParse(
      const std::string &                               key,
      ValueType                                         valueType,
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
