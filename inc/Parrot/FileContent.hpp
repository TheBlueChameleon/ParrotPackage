/* TODO: File descrption
 *
 */

#ifndef PARROT_FILECONTENT_HPP
#define PARROT_FILECONTENT_HPP

// ========================================================================== //
// dependencies

// STL
#include <string>
#include <any>

#include <vector>
#include <map>
#include <tuple>

// own
#include "Parrot/Definitions.hpp"
#include "Parrot/Reader.hpp"

// ========================================================================== //

namespace Parrot {

  // ======================================================================== //
  // class

  /**
   * @brief stores the parsed content of a file, together with interfaces for
   *    correctly-typed automated access
   *
   * Instances of this class are returned by the \c Parrot::Reader class
   */

  class FileContent {
  public:
    /**
     * @brief a container holding all data associated with a keyword
     *
     * This type is used internally and returned by the member function
     *    \c FileContent::get() and \c FileContent::getContent().
     *
     * For convenience, the indices are labelled in the enum
     *    \c FileContent::FileContentElements()
     *
     */
    using ContentType = std::tuple< std::any,             // value
                                    Parrot::ValueTypeID,  // value type
                                    bool,                 // found in file
                                    bool                  // triggered warning or error
                                  >;

  /**
   * @brief Names the indices to be used with std::get and the
   *    \c Parrot::ContentType
   */
  enum FileContentElements {
    Value,
    ValueType,
    FoundInFile,
    TriggeredWarning
  };

  private:
    std::string                        source = "<user defined>";
    std::map<std::string, ContentType> content;

    // ---------------------------------------------------------------------- //
    // safe getter

    ContentType                                 getSafe             (const std::string & key) const;

  public:
    // ---------------------------------------------------------------------- //
    // Type Converter Class

    /**
     * @brief Internally used struct to allow automated casting of an
     *    \c std::any to its specific C++ type (as specified by a
     *    \c Parrot::ValueTypeID())
     *
     * An object of this type is returned by
     *    <tt>TypeConverterClass Parrot::FileContent::operator[](const std::string &) const</tt>
     */

    struct TypeConverterClass {
      TypeConverterClass(const std::any & data) : data(data) {}

      std::any data;

      operator PARROT_TYPE(ValueTypeID::String     ) ();
      operator PARROT_TYPE(ValueTypeID::Integer    ) ();
      operator PARROT_TYPE(ValueTypeID::Real       ) ();
      operator PARROT_TYPE(ValueTypeID::Boolean    ) ();
      operator PARROT_TYPE(ValueTypeID::StringList ) ();
      operator PARROT_TYPE(ValueTypeID::IntegerList) ();
      operator PARROT_TYPE(ValueTypeID::RealList   ) ();
      operator PARROT_TYPE(ValueTypeID::BooleanList) ();
    };

    // ---------------------------------------------------------------------- //
    // CTors

    FileContent() = default;
    //! Constructs an empty \c Parrot::FileContent object and sets the source tag
    FileContent(const std::string & source);

    // ---------------------------------------------------------------------- //
    // Getters

    //! returns the name of the file from which the object was created
    const std::string &                         getSource           () const;

    //! returns \c true if the object does not hold any data
    bool                                        empty               () const;
    //! returns the number of keywords in the Parrot::FileContent object
    size_t                                      size                () const;

    //! returns \c true if \c key is a known keyword to the \c Parrot::FileContent object
    bool                                        hasKeyword          (const std::string & key) const;
    //! returns \c true if the value associated with \c key is not empty
    bool                                        hasValue            (const std::string & key) const;

    /**
     *
     */
    ContentType                                 get                 (const std::string & key) const;
    std::any                                    getAny              (const std::string & key) const;
    Parrot::ValueTypeID                         getValueType        (const std::string & key) const;
    bool                                        getFoundInFile      (const std::string & key) const;
    bool                                        getTriggeredWarning (const std::string & key) const;

    std::vector<std::string>                    getKeywords() const;

    const std::map<std::string, ContentType> &  getContent() const;

    // ---------------------------------------------------------------------- //
    // Value Access

    //! lerp
    TypeConverterClass                          operator[]          (const std::string & key) const;

    template <typename T>
    T getValue (const std::string & key) const;


    // ---------------------------------------------------------------------- //
    // Setters

    void reset();

    void addElement    (const std::string & key,
                        const std::any &    value            = std::any(),
                        bool                foundInFile      = false,
                        bool                triggeredWarning = false);

    void updateElement (const std::string & key,
                        const std::any &    value            = std::any(),
                        bool                foundInFile      = false,
                        bool                triggeredWarning = false);

    // ---------------------------------------------------------------------- //
    // Representation

    std::string to_string() const;
  };
}

// ========================================================================== //
// template implementations

template <typename T>
T Parrot::FileContent::getValue (const std::string & key) const {return std::any_cast<T>(getAny(key));}

// ========================================================================== //

#endif
