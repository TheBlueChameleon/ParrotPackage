/* TODO: File descrption
 * 
 */

#ifndef SETTINGS_DESCRIPTOR_HPP
#define SETTINGS_DESCRIPTOR_HPP

// ========================================================================== //
// dependencies

// STL
#include <string>
#include <any>

// own
#include "Settings/Definitions.hpp"

// ========================================================================== //
namespace Settings {  
  // ======================================================================== //
  // class

  class Descriptor {
  private:
  public:
    std::string   key;
    std::any      value;                                                        // default value, if 'key' is not in file
    ValueType     valueType = ValueType::Integer;
    
    bool          keyCaseSensitive        = false;
    bool          valueCaseSensitive      = false;
    
    bool          trimLeadingWhitespaces  = true;                               // only affects values, not keys
    bool          trimTrailingWhitespaces = true;
    
    bool          mandatory               = false;
    
  };
}
// ========================================================================== //
#endif
