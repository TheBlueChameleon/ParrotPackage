// ========================================================================== //
// local macro

#define THROWTEXT(msg) (std::string("RUNTIME EXCEPTION IN ") + (__PRETTY_FUNCTION__) + "\n" + msg)

// ========================================================================== //
// procs

// -------------------------------------------------------------------------- //
// type conversion

template<class T>
static inline const std::string BCG::complex_to_string(const std::complex<T> & z) {
  std::stringstream stream;
  stream << z;
  return stream.str();
}

// -------------------------------------------------------------------------- //
// vector norm and distance

template<class Iterator>
double BCG::norm_Euclidean(Iterator begin, Iterator end) {return std::sqrt(BCG::norm_modSquareSum(begin, end));}
// .......................................................................... //
template<class Iterator>
double BCG::norm_modSquareSum(Iterator begin, Iterator end) {
  return std::transform_reduce( begin, end,
                                begin,
                                0.0,
                                std::plus<double>(),
                                [] (
                                  const BCG::complex_d_t lhs,
                                  const BCG::complex_d_t rhs
                                ) {return std::real(lhs * std::conj(rhs));}
  );
}
// .......................................................................... //
template<class Iterator>
double BCG::norm_absSum(Iterator begin, Iterator end) {
  return std::transform_reduce(begin, end,
                               0.0,
                               std::plus<>(),
                               std::abs<double>
                              );
}
// .......................................................................... //
template<class Iterator>
double BCG::norm_max(Iterator begin, Iterator end) {
  return std::transform_reduce(begin, end,
                               0.0,
                               [] (const double & acc, const double & elm) {return std::max(acc, elm);},
                               std::abs<double>
                              );
}
// -------------------------------------------------------------------------- //

template<class Iterator>
double BCG::norm_Euclidean_real(Iterator begin, Iterator end) {return std::sqrt(BCG::norm_modSquareSum_real(begin, end));}
// .......................................................................... //
template<class Iterator>
double BCG::norm_modSquareSum_real(Iterator begin, Iterator end) {
  return std::transform_reduce( begin, end,
                                begin,
                                0.0
  );
}
// .......................................................................... //
template<class Iterator>
double BCG::norm_absSum_real(Iterator begin, Iterator end) {
  return std::transform_reduce(begin, end,
                               0.0,
                               std::plus<>(),
                               static_cast<double (*)(double)>(std::abs)
                              );
}
// -------------------------------------------------------------------------- //
template<class T>
double BCG::vector_distance(const std::vector<T> & A, const std::vector<T> & B) {
  auto l_add            = [] (const T a, const T b) {return a + b;};
  auto l_delta_squared  = [] (const T a, const T b) {T tmp = a - b; return tmp * tmp;};

  if ( A.size() != B.size() ) return T();

  return std::sqrt(
    std::inner_product( A.begin(), A.end(),
                        B.begin(),
                        0.0,
                        l_add,
                        l_delta_squared
    )
  );
}
// -------------------------------------------------------------------------- //
template <typename T>
static inline T BCG::factorial (T n) {
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

// ========================================================================== //

#undef THROWTEXT
