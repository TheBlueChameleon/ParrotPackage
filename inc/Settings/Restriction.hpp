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
#include "Settings/Definitions.hpp"

// ========================================================================== //
namespace Settings {
  
  // ======================================================================== //
  // class
  
  class Restriction {
  private:
    RestrictionType preParseRestrictionType = RestrictionType::None;
    std::any        preParseRestriction;
    
    RestrictionType aftParseRestrictionType = RestrictionType::None;
    std::any        aftParseRestriction;
    
  public:
    // ---------------------------------------------------------------------- //
    // CTors
    
//     Restriction();
    
    // ---------------------------------------------------------------------- //
    // Getters
    
    RestrictionType   getPreParseRestrictionType() const;
    const std::any &  getPreParseRestriction    () const;
    
    RestrictionType   getAftParseRestrictionType() const;
    const std::any &  getAftParseRestriction    () const;
    
    
    const std::vector<std::string>                  getPreParseList() const;
    const std::vector<std::string>                  getAftParseList () const;
    
    const std::pair<double, double>                 getPreParseRange() const;
    const std::pair<double, double>                 getAftParseRange() const;
    
    const std::function<bool (const std::string &)> getPreParseFunc() const;
    template<typename T>
    const std::function<bool (const T &)>           getAftParseFunc () const;
    
    // ---------------------------------------------------------------------- //
    // Setters
    
    void reset();
    void resetPreParseRestriction();
    void resetAftParseRestriction();
    
    void setPreParseList(const std::vector<std::string> & list, bool forbiddenList = false);
    void setAftParseList(const std::vector<std::string> & list, bool forbiddenList = false);
    
    // ---------------------------------------------------------------------- //
    // Representation
    
    std::string to_string() const;
  };
}
// ========================================================================== //
// template implementations

#include "Settings/Restriction.tpp"

// ========================================================================== //
#endif
