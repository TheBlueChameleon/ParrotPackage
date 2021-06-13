// ========================================================================== //
// local macro

#define THROWTEXT(msg) (std::string("RUNTIME EXCEPTION IN ") + (__PRETTY_FUNCTION__) + "\n" + msg)

// ========================================================================== //
// procs

// -------------------------------------------------------------------------- //
// convert

template <class InputIt>
static inline std::vector<typename std::iterator_traits<InputIt>::value_type>
BCG::to_vector (InputIt beg, InputIt end) {
  return std::vector(beg, end);
}

// -------------------------------------------------------------------------- //
// concatenate vectors

template<class InputIt>
static inline std::vector<typename std::iterator_traits<InputIt>::value_type>
BCG::concatenate (InputIt begA, InputIt endA,
                  InputIt begB, InputIt endB
) {
  std::vector<typename std::iterator_traits<InputIt>::value_type> reVal(begA, endA);

  reVal.insert( reVal.end(), begB, endB );

  return reVal;
}

template<class T>
inline std::vector<T> BCG::concatenate (const std::vector<T> & A, const std::vector<T> & B) {return BCG::concatenate(A.begin(), A.end(), B.begin(), B.end());}

// .......................................................................... //

template<class T>
static inline void BCG::appendTo_vector (std::vector<T> & A, const std::vector<T> & B) {
  A.reserve( A.size() + B.size() );
  A.insert ( A.end(), B.begin(), B.end() );
}

// -------------------------------------------------------------------------- //
// show lists of lists Py-Style

template<class InputIt>
static inline std::string BCG::vector_to_string(InputIt beg, InputIt end, bool brackets) {
  std::stringstream stream;
  std::string reVal;

  if (std::distance(beg, end) == 0u) {return brackets ? "[]" : "(empty)";}

  if (brackets) {stream << "[";}

  for (auto it = beg; it != end; ++it) {
    stream << *it << ",";
  }

  // remove the last comma
  stream.seekp(-1, stream.cur);
  stream << "]";

  reVal = stream.str();
  if (!brackets) {reVal.pop_back();}

  return reVal;
}
// .......................................................................... //
template<class T>
static inline std::string BCG::vector_to_string(const std::vector<T> & list, bool brackets) {return BCG::vector_to_string(list.begin(), list.end(), brackets);}
// .......................................................................... //
template<class T>
static inline std::string BCG::vecvec_to_string(const std::vector<std::vector<T>> & listlist) {
  std::stringstream reVal;

  if (listlist.size() == 0u) {return "[.]";}

  reVal << "[";
  for   (auto & e : listlist) {
    reVal << vector_to_string<T>(e) << ",";
  }

  // remove the last comma
  reVal.seekp(-1, reVal.cur);

  reVal << "]";

  return reVal.str();
}

// -------------------------------------------------------------------------- //
// find nearby

template<class Iterator, class T>
Iterator BCG::findNearby(Iterator begin, Iterator end,
                    const T & value,
                    double epsilon,
                    std::function<double(T)> absfunc,
                    std::function<T(T, T)> difffunc
) {
  auto it = begin;
  for (; it != end; ++it) {
    if (absfunc( difffunc(*it, value)) < epsilon) {break;}
  }

  return it;
}
// .......................................................................... //
template<typename Iterator, typename T>
int BCG::findNearbyIdx(Iterator begin, Iterator end,
                       const T & value,
                       double epsilon,
                       std::function<double(T)> absfunc,
                       std::function<T(T, T)>   difffunc
) {
  auto spot = findNearby(begin, end, value, epsilon, absfunc, difffunc);
  if (spot == end) {return -1;}
  else             {return std::distance(begin, spot);}
}

// ============================================================================ //

#undef THROWTEXT
