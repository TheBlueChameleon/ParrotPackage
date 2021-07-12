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
  // local constants

  enum FileContentElements {
    FCE_Value,
    FCE_ValueType,
    FCE_FoundInFile,
    FCE_TriggeredWarning
  };

  // ======================================================================== //
  // class

  class FileContent {
  public:
    using ContentType = std::tuple< std::any,             // value
                                    Parrot::ValueTypeID,  // value type
                                    bool,                 // found in file
                                    bool                  // triggered warning or error
                                  >;

    friend Parrot::Reader;

  private:
    std::string                        source = "<user defined>";
    std::map<std::string, ContentType> content;

    // ---------------------------------------------------------------------- //
    // content manipulation


  public:
    // ---------------------------------------------------------------------- //
    // Type Converter Class

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
    FileContent(const std::string & source);

    // ---------------------------------------------------------------------- //
    // Getters

    const std::string &                         getSource           () const;

    bool                                        empty               () const;
    size_t                                      size                () const;

    bool                                        hasKeyword          (const std::string & key) const;
    bool                                        hasValue            (const std::string & key) const;

    ContentType                                 get                 (const std::string & key) const;
    std::any                                    getAny              (const std::string & key) const;
    Parrot::ValueTypeID                         getValueType        (const std::string & key) const;
    bool                                        getFoundInFile      (const std::string & key) const;
    bool                                        getTriggeredWarning (const std::string & key) const;

    std::vector<std::string>                    getKeywords() const;

    const std::map<std::string, ContentType> &  getContent() const;

    // ---------------------------------------------------------------------- //
    // Value Access

    TypeConverterClass                    operator[]          (const std::string & key) const;

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
