// ========================================================================= //
// dependencies
// STL
#include <stdexcept>

#include <vector>
#include <any>
#include <initializer_list>

// own
#include "Settings/Definitions.hpp"

// ========================================================================== //
// local macro

#define THROWTEXT(msg) ("RUNTIME EXCEPTION IN "s + (__PRETTY_FUNCTION__) + "\n"s + msg)

// ========================================================================== //
namespace Settings {
  // ======================================================================== //
  // Setters

  template<typename T>
  void Descriptor::setValue(T newVal, bool resetMetaData) {
    if      ( std::is_same<bool,                              T>::value ) {valueType = ValueType::Boolean    ;}
    else if ( std::is_integral<                               T>::value ) {valueType = ValueType::Integer    ;}
    else if ( std::is_floating_point<                         T>::value ) {valueType = ValueType::Real       ;}
    else if ( std::is_constructible<std::string,              T>::value ) {valueType = ValueType::String     ;}
    else if ( std::is_constructible<std::vector<std::string>, T>::value ) {valueType = ValueType::StringList ;}
    else if ( std::is_constructible<std::vector<bool>,        T>::value ) {valueType = ValueType::BooleanList;}
    else if ( std::is_constructible<std::vector<int>,         T>::value ) {valueType = ValueType::IntegerList;}
    else if ( std::is_constructible<std::vector<double>,      T>::value ) {valueType = ValueType::RealList   ;}
    else {throw std::invalid_argument(std::string(__PRETTY_FUNCTION__) + "\nType not supported.");}
    
    value = newVal;
    
    if (resetMetaData) {
      restrictions.clear();
      substitutions.clear();
      userPreParser = nullptr;
    }
  }
  // ........................................................................ //
  template<typename T>
  void Descriptor::setValue(const std::initializer_list<T> & list, bool resetMetaData) {setValue(std::vector<T>(list), resetMetaData);}
  
  // ------------------------------------------------------------------------ //
}
// ========================================================================== //
#undef THROWTEXT
