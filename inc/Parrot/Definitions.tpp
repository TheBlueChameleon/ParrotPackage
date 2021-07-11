// ========================================================================= //
// dependencies

// STL
#include <stdexcept>

// ========================================================================== //
// local macro

#define THROWTEXT(msg) (std::string("RUNTIME EXCEPTION IN ") + (__PRETTY_FUNCTION__) + "\n" + msg)

// ========================================================================== //

namespace Parrot {

  // ======================================================================== //
  // type interpreters

  template<typename T>
  constexpr ValueTypeID valueTypeIDOf([[maybe_unused]] const T & x) {
    if      ( std::is_same<bool,                               T>::value ) {return ValueTypeID::Boolean    ;}
    else if ( std::is_integral<                                T>::value ) {return ValueTypeID::Integer    ;}
    else if ( std::is_floating_point<                          T>::value ) {return ValueTypeID::Real       ;}
    else if ( std::is_constructible<std::string,               T>::value ) {return ValueTypeID::String     ;}
    else if ( std::is_constructible<std::vector<std::string>,  T>::value ) {return ValueTypeID::StringList ;}
    else if ( std::is_constructible<std::vector<const char *>, T>::value ) {return ValueTypeID::StringList ;}
    else if ( std::is_constructible<std::vector<bool>,         T>::value ) {return ValueTypeID::BooleanList;}
    else if ( std::is_constructible<std::vector<int>,          T>::value ) {return ValueTypeID::IntegerList;}
    else if ( std::is_constructible<std::vector<double>,       T>::value ) {return ValueTypeID::RealList   ;}
    else {throw std::invalid_argument(THROWTEXT("    Type not supported."));}
  }
  // ........................................................................ //
  template<typename T>
  constexpr ValueTypeID valueTypeIDOf(const std::initializer_list<T> & x) {return valueTypeIDOf(std::vector<T>(x));}

}

// ========================================================================== //

#undef THROWTEXT
