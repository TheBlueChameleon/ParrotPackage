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
    
    std::vector<Restriction>                          restrictions;
    
    std::vector<std::pair<std::string, std::string>>  substitutions;            // defines a dictionary for the first step of the parsing process
    std::function<std::string (const std::string &)>  userParser;               // will be called after doing the substitutions with the read value as an argument. Output is user-parsed line
    
  public:
    // ---------------------------------------------------------------------- //
    // CTors
    
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
    
    const std::vector<Restriction>                          & getRestrictions() const;

    const std::vector<std::pair<std::string, std::string>>  & getSubstitutions() const;
    const std::function<std::string (const std::string &)>  & getUserParser   () const;
    
    // ---------------------------------------------------------------------- //
    // Setters
    
    void reset ();
    
    void setKey (const std::string & newVal);
    
    
    
    void setKeyCaseSensitive        (bool newVal);
    void setValueCaseSensitive      (bool newVal);

    void setTrimLeadingWhitespaces  (bool newVal);
    void setTrimTrailingWhitespaces (bool newVal);

    void setMandatory               (bool newVal);
    
    // ---------------------------------------------------------------------- //
    // Representation
    
    std::string to_string() const;
  };
}
// ========================================================================== //
#endif
