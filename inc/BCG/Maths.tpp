// ========================================================================= //
// dependencies

// STL
#include <stdexcept>

#include <sstream>

// ========================================================================== //
// local macro

#define THROWTEXT(msg) (std::string("RUNTIME EXCEPTION IN ") + (__PRETTY_FUNCTION__) + "\n" + msg)

// ========================================================================== //

namespace BCG {
  template<class T>
  static inline const std::string complex_to_string(const std::complex<T> & z) {
    std::stringstream stream;
    stream << z;
    return stream.str();
  }
  // ------------------------------------------------------------------------ //
  template <typename T>
  static inline T factorial (T n) {
    /* Can be used with GMP types and works just fine. However, GMP classes have
     * their proper methods for this.
     *
     * type unsigned long long int yields correct results up to n = 20 and then
     * goes into overload (returns zero)
     * type double yields good results up to n = 22
     *    reaches computation limit after n = 170 and will yield nan:inf beyond
     *    (nothrow)
     *    use at own risk above that
     * type long double yields good results up to n = 25
     */

    if (n <   0) {throw std::invalid_argument(THROWTEXT("    only positive arguments allowed."));}

    T reVal = 1.0;
    for (T i = 2; i < n + 1; ++i) {reVal *= i;}

    return reVal;
  }
}

// ========================================================================== //

#undef THROWTEXT
