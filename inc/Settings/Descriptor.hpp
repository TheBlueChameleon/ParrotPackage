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
#include "Settings/Definitions.hpp"
#include "Settings/Restriction.hpp"

// ========================================================================== //
namespace Settings {  
  // ======================================================================== //
  // class

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
    
  public:
    // ---------------------------------------------------------------------- //
    // CTors
    
    Descriptor() = default;
    Descriptor(std::string key, ValueType valueType = ValueType::Integer, bool mandatory = false);
    
    // ---------------------------------------------------------------------- //
    // Getters
    
    std::string   getKey      () const;
    std::any      getValue    () const;
    ValueType     getValueType() const;
    
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
    void setValue(T newVal, bool resetMetaData = true);
    
    template<typename T>
    void setValue(const std::initializer_list<T> & list, bool resetMetaData = true);
    
    void setKeyCaseSensitive        (bool newVal);
    void setValueCaseSensitive      (bool newVal);

    void setTrimLeadingWhitespaces  (bool newVal);
    void setTrimTrailingWhitespaces (bool newVal);

    void setMandatory               (bool newVal);
    
    void addRestriction  (const Restriction & newVal);
    void addSubstitution (const std::string & substituee, const std::string & substitute);
    void addUserPreParser(const std::function<const std::string & (const std::string &)> & newVal);
    
    void makeRanged(const std::string & key,
                    double min, double max,
                    ValueType valueType = ValueType::Integer,
                    std::any defaultValue = std::any(),
                    RestrictionViolationPolicy policy = RestrictionViolationPolicy::Exception,
                    const std::string & restrictionViolationText = "value out of bounds",
                    bool mandatory = false
                   );
    
    // ---------------------------------------------------------------------- //
    // Representation
    
    std::string to_string() const;
  };
}
// ========================================================================== //
// template implementations

#include "Settings/Descriptor.tpp"

// ========================================================================== //
#endif
