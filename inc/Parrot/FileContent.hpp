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

    ContentType getSafe (const std::string & key) const;

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

    // ...................................................................... //

    //! returns \c true if \c key is a known keyword to the \c Parrot::FileContent object
    bool                                        hasKeyword          (const std::string & key) const;
    //! returns \c true if the value associated with \c key is not empty
    bool                                        hasValue            (const std::string & key) const;
    //! returns all keyword names as a \c std::vector
    std::vector<std::string>                    getKeywords() const;

    //! returns the \c std::map underlying the \c Parrot::FileContent object
    const std::map<std::string, ContentType> &  getContent() const;
    // ...................................................................... //

    /**
     * @brief returns the entire content associated with a keyword as a
     *    \c Parrot::FileContent::ContentType object
     *
     * @throws Parrot::ValueAccessError if the requested keyword is not part of the
     *    recorded data
     */
    ContentType                                 get                 (const std::string & key) const;
    /**
     * @brief returns the keyword value as a std::any object
     *
     * @throws std::out_of_range if the requested keyword is not part of the
     *    recorded data
     */
    std::any                                    getAny              (const std::string & key) const;
    /**
     * @brief returns the \c Parrot::ValueTypeID() associated with the keyword
     *
     * @throws std::out_of_range if the requested keyword is not part of the
     *    recorded data
     */
    Parrot::ValueTypeID                         getValueType        (const std::string & key) const;
    /**
     * @brief returns whether the keyword was actually found in the file (or
     *    whether its default value was used instead)
     *
     * @throws std::out_of_range if the requested keyword is not part of the
     *    recorded data
     */
    bool                                        getFoundInFile      (const std::string & key) const;
    /**
     * @brief returns whether the keyword triggered a warning in the parsing
     *    process
     *
     * @throws std::out_of_range if the requested keyword is not part of the
     *    recorded data
     */
    bool                                        getTriggeredWarning (const std::string & key) const;


    // ---------------------------------------------------------------------- //
    // Value Access

    /**
     * @brief get the value associated with a keyword and explicitly cast it to
     *    a *Parrot* string
     *
     * This is the explicit form of invoking a \c static_cast on the value
     *    returned by the <tt>Parrot::FileContent::operator[](const std::string &) const</tt>
     *
     * @throws std::out_of_range if the requested keyword is not part of the
     *    recorded data
     */
    PARROT_TYPE(ValueTypeID::String     )       get_String          (const std::string & key) const;
    /**
     * @brief get the value associated with a keyword and explicitly cast it to
     *    a *Parrot* integer
     *
     * This is the explicit form of invoking a \c static_cast on the value
     *    returned by the <tt>Parrot::FileContent::operator[](const std::string &) const</tt>
     *
     * @throws std::out_of_range if the requested keyword is not part of the
     *    recorded data
     */
    PARROT_TYPE(ValueTypeID::Integer    )       get_Integer         (const std::string & key) const;
    /**
     * @brief get the value associated with a keyword and explicitly cast it to
     *    a *Parrot* real
     *
     * This is the explicit form of invoking a \c static_cast on the value
     *    returned by the <tt>Parrot::FileContent::operator[](const std::string &) const</tt>
     *
     * @throws std::out_of_range if the requested keyword is not part of the
     *    recorded data
     */
    PARROT_TYPE(ValueTypeID::Real       )       get_Real            (const std::string & key) const;
    /**
     * @brief get the value associated with a keyword and explicitly cast it to
     *    a *Parrot* boolean
     *
     * This is the explicit form of invoking a \c static_cast on the value
     *    returned by the <tt>Parrot::FileContent::operator[](const std::string &) const</tt>
     *
     * @throws std::out_of_range if the requested keyword is not part of the
     *    recorded data
     */
    PARROT_TYPE(ValueTypeID::Boolean    )       get_Boolean         (const std::string & key) const;
    /**
     * @brief get the value associated with a keyword and explicitly cast it to
     *    a *Parrot* string list
     *
     * This is the explicit form of invoking a \c static_cast on the value
     *    returned by the <tt>Parrot::FileContent::operator[](const std::string &) const</tt>
     *
     * @throws std::out_of_range if the requested keyword is not part of the
     *    recorded data
     */
    PARROT_TYPE(ValueTypeID::StringList )       get_StringList      (const std::string & key) const;
    /**
     * @brief get the value associated with a keyword and explicitly cast it to
     *    a *Parrot* integer list
     *
     * This is the explicit form of invoking a \c static_cast on the value
     *    returned by the <tt>Parrot::FileContent::operator[](const std::string &) const</tt>
     *
     * @throws std::out_of_range if the requested keyword is not part of the
     *    recorded data
     */
    PARROT_TYPE(ValueTypeID::IntegerList)       get_IntegerList     (const std::string & key) const;
    /**
     * @brief get the value associated with a keyword and explicitly cast it to
     *    a *Parrot* real list
     *
     * This is the explicit form of invoking a \c static_cast on the value
     *    returned by the <tt>Parrot::FileContent::operator[](const std::string &) const</tt>
     *
     * @throws std::out_of_range if the requested keyword is not part of the
     *    recorded data
     */
    PARROT_TYPE(ValueTypeID::RealList   )       get_RealList        (const std::string & key) const;
    /**
     * @brief get the value associated with a keyword and explicitly cast it to
     *    a *Parrot* boolean list
     *
     * This is the explicit form of invoking a \c static_cast on the value
     *    returned by the <tt>Parrot::FileContent::operator[](const std::string &) const</tt>
     *
     * @throws std::out_of_range if the requested keyword is not part of the
     *    recorded data
     */
    PARROT_TYPE(ValueTypeID::BooleanList)       get_BooleanList     (const std::string & key) const;


    /**
     * @brief returns the keyword value in a form that can automatically be
     *    casted to its proper C++ type
     *
     * The returned \c Parrot::FileContent::TypeConverterClass object has
     *    casting operators associated that are implicitly called depending on
     *    context. For variable assignment, the data type of the assignee
     *    determines the selected casting operator:
     * @code
     * PARROT_TYPE( Parrot::ValueTypeID::Integer ) assignee;
     * assignee = fileContentObject["keyword"];
     * @endcode
     *
     * Where this is ambiguous, a static_cast<>() can be used:
     * @code
     * short assignee;
     * assignee = static_cast<PARROT_TYPE(Parrot::ValueTypeID::Integer)>( fileContentObject["keyword"] );
     * @endcode
     *
     * This is equivalent to using the explicit type getters:
     * @code
     * short assignee;
     * assignee fileContentObject.get_Integer("keyword");
     * @endcode
     *
     * Using the template function \c FileContent::getValue() causes longer
     *    compile times but is faster at runtime due to fewer function calls:
     * @code
     * short assignee;
     * assignee fileContentObject.getValue<PARROT_TYPE(Parrot::ValueTypeID::Integer>("keyword");
     * @endcode
     */
    TypeConverterClass                          operator[]          (const std::string & key) const;

    /**
     * @brief returns the value associated with a keyword, cast to type \c T
     *
     * See <tt>Parrot::FileContent::operator[](const std::string &) const</tt>
     *    for an example.
     *
     * @note This template was added to give quick remedies in case some missing
     *    implementation of later-added types causes trouble. Stick to the
     *    automated cast operator of \c Parrot::FileContent::TypeConverterClass
     *    or use the explicit type casts like \c FileContent::get_Integer().
     *
     * @throws std::bad_any_cast if T does not match the content
     */
    template <typename T>
    T getValue (const std::string & key) const;

    // ---------------------------------------------------------------------- //
    // Setters

    //! restores the state as if created from the empty CTor.
    void reset();

    /**
     * @brief adds a keyword and an associated value or throws an error if the
     *    keyword is already in the \c Parrot::FileContent object
     *
     * @note \c value must be of a type as specified by \c Parrot::ValueTypeID
     * @note to specify a keyword with empty content, use the overloaded form
     *    <tt>Parrot::FileContent::addElement(const std::string &, const ValueTypeID, bool, bool)</tt>
     * @note to update an existing keyword, use \c FileContent::updateElement()
     *    or \c FileContent::setElement()
     *
     * @throws Parrot::ValueAccessError if keyword already exists
     * @throws Parrot::ValueTypeError if type of \c value is not a *Parrot* type
     * @throws Parrot::ValueTypeError if \c value has no value
     */
    void addElement    (const std::string & key,
                        const std::any &    value,
                        bool                foundInFile      = false,
                        bool                triggeredWarning = false);
    /**
     * @brief adds a keyword and an associated value type with empty keyword
     *    value, or throws an error if the keyword is already in the
     *    \c Parrot::FileContent object
     *
     * @note to specify a keyword with a keyword value, use the overloaded form
     *    <tt>Parrot::FileContent::addElement(const std::string &, const std::any &, bool, bool)</tt>
     * @note to update an existing keyword, use \c FileContent::updateElement()
     *    or \c FileContent::setElement()
     *
     * @throws Parrot::ValueAccessError if keyword already exists
     */
    void addElement    (const std::string & key,
                        const ValueTypeID   valueType,
                        bool                foundInFile      = false,
                        bool                triggeredWarning = false);

    // ...................................................................... //

    /**
     * @brief updates a keyword and associated value or throws an error if the
     *      keyword does not exist yet
     *
     * @note to specify a keyword with empty content, use the overloaded form
     *    <tt>Parrot::FileContent::updateElement(const std::string &, const ValueTypeID, bool, bool)</tt>
     * @note to add an not-yet existing keyword, use \c FileContent::addElement()
     *    or \c FileContent::setElement()
     *
     * @throws Parrot::ValueAccessError if keyword does not exist yet
     * @throws Parrot::ValueTypeError if type of \c value is not a *Parrot* type
     * @throws Parrot::ValueTypeError if \c value has no value
     */
    void updateElement (const std::string & key,
                        const std::any &    value,
                        bool                foundInFile      = false,
                        bool                triggeredWarning = false);
    /**
     * @brief updates a keyword and associated value type with empty keyword
     *    value, or throws an error if the keyword does not exist yet
     *
     * @note \c value must be of a type as specified by \c Parrot::ValueTypeID
     * @note to specify a keyword with a keyword value, use the overloaded form
     *    <tt>Parrot::FileContent::updateElement(const std::string &, const std::any &, bool, bool)</tt>
     * @note to add an not-yet existing keyword, use \c FileContent::addElement()
     *    or \c FileContent::setElement()
     *
     * @throws Parrot::ValueAccessError if keyword does not exist yet
     */
    void updateElement (const std::string & key,
                        const ValueTypeID   valueType,
                        bool                foundInFile      = false,
                        bool                triggeredWarning = false);

    // ...................................................................... //

    /**
     * @brief adds or updates a keyword and an associated value
     *
     * @note \c value must be of a type as specified by \c Parrot::ValueTypeID
     * @note to specify a keyword with empty content, use the overloaded form
     *    <tt>Parrot::FileContent::setElement(const std::string &, const ValueTypeID, bool, bool)</tt>
     * @note to check whether the keyword already exists, use
     *    \c FileContent::updateElement() or \c FileContent::addElement()
     *
     * @throws Parrot::ValueTypeError if type of \c value is not a *Parrot* type
     * @throws Parrot::ValueTypeError if \c value has no value
     */
    void setElement    (const std::string & key,
                        const std::any &    value,
                        bool                foundInFile      = false,
                        bool                triggeredWarning = false);
    /**
     * @brief adds or updates a keyword and an associated value
     *
     * @note to specify a keyword with a keyword value, use the overloaded form
     *    <tt>Parrot::FileContent::setElement(const std::string &, const std::any &, bool, bool)</tt>
     * @note to check whether the keyword already exists, use
     *    \c FileContent::updateElement() or \c FileContent::addElement()
     */
    void setElement    (const std::string & key,
                        const ValueTypeID   valueType,
                        bool                foundInFile      = false,
                        bool                triggeredWarning = false);

    // ---------------------------------------------------------------------- //
    // Representation

    /**
     * @brief creates a string with a tabular representation of the content
     *
     * Example:
     * The following lines:
     * @code
     * Parrot::FileContent fc("### DEBUG FILE ###");
     * fc.addElement(integer, 42ll);
     * fc.addElement(string, "std::string"s);
     * fc.addElement(empty, Parrot::ValueTypeID::Boolean);
     * std::cout << fc.to_string() << std::endl;
     * @endcode
     * cause the output:
     @verbatim
     keyword |       content        | data type | in File | triggered Warning
     --------+----------------------+-----------+---------+------------------
     empty   | (### no content ###) | boolean   |   no    |        no
     integer | 42                   | integer   |   no    |        no
     string  | std::string          | string    |   no    |        no
     @endverbatim
     *
     * the width of the table columns is automatically set to fit their content
     */
    std::string to_string() const;
  };
}

// ========================================================================== //
// template implementations

template <typename T>
T Parrot::FileContent::getValue (const std::string & key) const {return std::any_cast<T>(getAny(key));}

// ========================================================================== //

#endif
