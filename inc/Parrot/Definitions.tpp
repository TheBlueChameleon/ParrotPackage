// ========================================================================= //
// dependencies

// STL
#include <stdexcept>

// ========================================================================== //
// local macro

#define THROWTEXT(msg) (std::string("RUNTIME EXCEPTION IN ") + (__PRETTY_FUNCTION__) + "\n" + msg)

// ========================================================================== //
// type interpreters

template<typename T>
constexpr Parrot::ValueTypeID Parrot::valueTypeIDOf([[maybe_unused]] const T & x) {
  if      ( std::is_same          <PARROT_TYPE(Parrot::ValueTypeID::Boolean    ), T>::value ) {return Parrot::ValueTypeID::Boolean    ;}
  else if ( std::is_integral      <                                               T>::value ) {return Parrot::ValueTypeID::Integer    ;}
  else if ( std::is_floating_point<                                               T>::value ) {return Parrot::ValueTypeID::Real       ;}
  else if ( std::is_constructible <PARROT_TYPE(Parrot::ValueTypeID::String     ), T>::value ) {return Parrot::ValueTypeID::String     ;}
  else if ( std::is_constructible <PARROT_TYPE(Parrot::ValueTypeID::StringList ), T>::value ) {return Parrot::ValueTypeID::StringList ;}
  else if ( std::is_constructible <std::vector<const char *>,                     T>::value ) {return Parrot::ValueTypeID::StringList ;}
  else if ( std::is_constructible <PARROT_TYPE(Parrot::ValueTypeID::BooleanList), T>::value ) {return Parrot::ValueTypeID::BooleanList;}
  else if ( std::is_constructible <PARROT_TYPE(Parrot::ValueTypeID::IntegerList), T>::value ) {return Parrot::ValueTypeID::IntegerList;}
  else if ( std::is_constructible <PARROT_TYPE(Parrot::ValueTypeID::RealList   ), T>::value ) {return Parrot::ValueTypeID::RealList   ;}
  else {throw Parrot::ValueTypeError(THROWTEXT("    Type not supported."));}
}
// ........................................................................ //
template<typename T>
constexpr Parrot::ValueTypeID Parrot::valueTypeIDOf(const std::initializer_list<T> & x) {return valueTypeIDOf(std::vector<T>(x));}


// ========================================================================== //

#undef THROWTEXT
