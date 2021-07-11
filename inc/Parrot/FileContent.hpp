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
    // CTors

    FileContent() = default;
    FileContent(const std::string & key,
                const std::any &    value,
                bool                foundInFile = true,
                bool                triggeredWarning = false
    );

    // ---------------------------------------------------------------------- //
    // Getters

    std::string         getSource  () const;

    bool                hasKeyword          (const std::string & key) const;
    bool                hasValue            (const std::string & key) const;
    ContentType         get                 (const std::string & key) const;
    std::any            getValue            (const std::string & key) const;
    Parrot::ValueTypeID getValueType        (const std::string & key) const;
    bool                getFoundInFile      (const std::string & key) const;
    bool                getTriggeredWarning (const std::string & key) const;

    std::vector<std::string> getKeywords() const;

    // ---------------------------------------------------------------------- //
    // Setters

    void addElement (const std::string & key,
                     const std::any &    value,
                     bool                foundInFile = true,
                     bool                triggeredWarning = false);

    // ---------------------------------------------------------------------- //
    // Random Access

    PARROT_TYPE(Parrot::ValueTypeID::String) & operator[] (const std::string & key);

    // ---------------------------------------------------------------------- //
    // Representation

    std::string to_string() const;
  };
}

// ========================================================================== //

#endif
