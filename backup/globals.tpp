// ========================================================================= //
// dependencies

// STL
#include <stdexcept>
#include <unistd.h>

#include "BCG.hpp"

// ========================================================================== //
// local macro

#define THROWTEXT(msg) (std::string("RUNTIME EXCEPTION IN ") + (__PRETTY_FUNCTION__) + "\n" + msg)

// ========================================================================== //

namespace BCG {
  // ======================================================================== //
  // console output convenience



  // ======================================================================== //
  // String utility



  // ------------------------------------------------------------------------ //
  // complex


  // ======================================================================== //
  // vector utility

  // ------------------------------------------------------------------------ //
  // convert

  template <class InputIt>
  static inline std::vector<typename std::iterator_traits<InputIt>::value_type> to_vector (
    InputIt beg,
    InputIt end
  ) {
    return std::vector(beg, end);
  }

  // ------------------------------------------------------------------------ //
  // concatenate vectors
  // https://stackoverflow.com/questions/3177241/what-is-the-best-way-to-concatenate-two-vectors

  template<class T>
  inline std::vector<T> concatenate (const std::vector<T> & A, const std::vector<T> & B) {
    std::vector<T> reVal;

    reVal.insert( reVal.end(), A.begin(), A.end() );
    reVal.insert( reVal.end(), B.begin(), B.end() );

    return reVal;
  }

  template<class InputIt>
  static inline std::vector<typename std::iterator_traits<InputIt>::value_type> concatenate (
    InputIt begA, InputIt endA,
    InputIt begB, InputIt endB
  ) {
    std::vector<typename std::iterator_traits<InputIt>::value_type> reVal(begA, endA);

    reVal.insert( reVal.end(), begB, endB );

    return reVal;
  }

  // ........................................................................ //

  template<class T>
  static inline void appendTo_vector (std::vector<T> & A, const std::vector<T> & B) {
    A.reserve( A.size() + B.size() );
    A.insert ( A.end(), B.begin(), B.end() );
  }

  // ------------------------------------------------------------------------ //
  // show lists of lists Py-Style

  template<class T>
  static inline std::string vector_to_string(const std::vector<T> & list, bool brackets) {
    std::stringstream stream;
    std::string reVal;

    if (list.size() == 0u) {return brackets ? "[]" : "(empty)";}

    if (brackets) {stream << "[";}

    for (auto & e : list) {
      stream << e << ",";
    }

    // remove the last comma
    stream.seekp(-1, stream.cur);
    stream << "]";

    reVal = stream.str();
    if (!brackets) {reVal.pop_back();}

    return reVal;
  }
  // ........................................................................ //
  template<class T>
  static inline std::string vecvec_to_string(const std::vector<std::vector<T>> & listlist) {
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

  // ------------------------------------------------------------------------ //
  // vector distance

  template<class T>
  double vector_distance(const std::vector<T> & A, const std::vector<T> & B) {
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

  // ------------------------------------------------------------------------ //
  // find nearby

  template<class Iterator, class T>
  Iterator findNearby(Iterator begin, Iterator end,
                      const T & val,
                      double epsilon,
                      double absFunc (double)
  ) {
    auto it = begin;
    for (; it != end; ++it) {
      if (absFunc(*it - val) < epsilon) {break;}
    }

    return it;
  }
  // ........................................................................ //
  template<class Iterator, class T>
  int findNearbyIdx(Iterator begin, Iterator end,
                      const T & val,
                      double epsilon,
                      double absFunc (double)
  ) {
    auto spot = findNearby(begin, end, val, epsilon, absFunc);
    if (spot == end) {return -1;}
    else             {return std::distance(begin, spot);}
  }

  // ======================================================================== //
  // File utilty

  static inline std::fstream openThrow(const std::string & filename, std::ios_base::openmode mode) {
    auto reVal = std::fstream(filename, mode);

    if ( !reVal.is_open() ) {
      throw( std::invalid_argument("failed to open '" + filename + "'") );
    }

    return reVal;
  }
  // ........................................................................ //
  static inline std::string  generateFileComments(const std::string & content) {
    std::string reVal;

    reVal += "# ============================================================================ #\n";
    auto lines = BCG::splitString(content, '\n');
    for (const auto & line : lines) {reVal += "# " + line + "\n";}
    reVal += "# timestamp: ";
    reVal +=    generateTimestamp() + "\n";
    reVal += "# ============================================================================ #\n\n";

    return reVal;
  }
}


// ========================================================================== //

#undef THROWTEXT