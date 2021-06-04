// ========================================================================= //
// dependencies
// STL
#include <stdexcept>
// ========================================================================== //
// local macro

#define THROWTEXT(msg) (std::string("RUNTIME EXCEPTION IN ") + (__PRETTY_FUNCTION__) + "\n" + msg)

// ========================================================================== //
namespace Settings {
  // ======================================================================== //
  // lookups

  template<typename T>
  ValueType getValueType([[maybe_unused]] const T & x) {
    if      ( std::is_same<bool,                              T>::value ) {return ValueType::Boolean    ;}
    else if ( std::is_integral<                               T>::value ) {return ValueType::Integer    ;}
    else if ( std::is_floating_point<                         T>::value ) {return ValueType::Real       ;}
    else if ( std::is_constructible<std::string,              T>::value ) {return ValueType::String     ;}
    else if ( std::is_constructible<std::vector<std::string>, T>::value ) {return ValueType::StringList ;}
    else if ( std::is_constructible<std::vector<bool>,        T>::value ) {return ValueType::BooleanList;}
    else if ( std::is_constructible<std::vector<int>,         T>::value ) {return ValueType::IntegerList;}
    else if ( std::is_constructible<std::vector<double>,      T>::value ) {return ValueType::RealList   ;}
    else {throw std::invalid_argument(THROWTEXT("    Type not supported."));}
  }
  // ........................................................................ //
  template<typename T>
  ValueType getValueType(const std::initializer_list<T> & x) {return getValueType(std::vector<T>(x));}

}
// ========================================================================== //
#undef THROWTEXT
