/* TODO: File descrption
 *
 */

#ifndef PARROT_READER_HPP
#define PARROT_READER_HPP

// ========================================================================== //
// dependencies

// STL
#include <string>
#include <vector>

// own
#include "Parrot/Definitions.hpp"
#include "Parrot/Descriptor.hpp"
#include "Parrot/FileContent.hpp"

// ========================================================================== //

namespace Parrot {

  // ======================================================================== //
  // class

  /**
   * @brief Reader
   * @todo Reader: Description
   */
  class Reader {
  private:
    // ...................................................................... //
    // state variables

    char                            commentMarker                   = '#';
    char                            multilineMarker                 = '\\';
    bool                            verbose                         = true;

    MissingKeywordPolicy            missingKeywordPoliyNonMandatory = MissingKeywordPolicy::Exception;
    std::string                     missingKeywordTextNonMandatory  = "keyword $K was not found; reverting to default ($D)";
    MissingKeywordPolicy            missingKeywordPolicyMandatory   = MissingKeywordPolicy::Warning;
    std::string                     missingKeywordTextMandatory     = "mandatory keyword $K was not found in file $F!";
    MissingKeywordPolicy            unexpectedKeywordPolicy         = MissingKeywordPolicy::Warning;
    std::string                     unexpectedKeywordText           = "unexpected keyword in file $F! (Taken as string keyword)\n$L";

    std::vector<Parrot::Descriptor> descriptors;

    // ...................................................................... //
    // parsing metastate variables


    // ...................................................................... //
    // parsing machinery

    void descriptorValidityCheck(const Parrot::Descriptor & descriptor) const;

  public:
    // ---------------------------------------------------------------------- //
    // CTors

    Reader() = default;
    Reader(const std::vector<Parrot::Descriptor> & descriptors);

    // ---------------------------------------------------------------------- //
    // Getters

    char                                    getCommentMarker  () const;
    char                                    getMultilineMarker() const;
    bool                                    getVerbose        () const;

    const MissingKeywordPolicy &            getMissingKeywordPolicyMandatory  () const;
    const std::string          &            getMissingKeywordTextMandatory    () const;
    const MissingKeywordPolicy &            getMissingKeywordPoliyNonMandatory() const;
    const std::string          &            getMissingKeywordTextNonMandatory () const;
    const MissingKeywordPolicy &            getUnexpectedKeywordPolicy        () const;
    const std::string          &            getUnexpectedKeywordText          () const;

    size_t                                  size      () const;
    bool                                    hasKeyword(const std::string & keyword) const;

    const std::vector<Parrot::Descriptor> & getDescriptors() const;
    //! @throws std::runtime_error if index out of bounds
    const             Parrot::Descriptor  & getDescriptor (const size_t        idx    ) const;
    //! @throws std::runtime_error if keyword does not exist
    const             Parrot::Descriptor  & getDescriptor (const std::string & keyword) const;

    // ---------------------------------------------------------------------- //
    // Setters

    void reset();
    void resetKeywords();

    void setMissingKeywordPolicyMandatory  (const MissingKeywordPolicy & newVal);
    void setMissingKeywordTextMandatory    (const std::string          & newVal);
    void setMissingKeywordPoliyNonMandatory(const MissingKeywordPolicy & newVal);
    void setMissingKeywordTextNonMandatory (const std::string          & newVal);
    void setUnexpectedKeywordPolicy        (const MissingKeywordPolicy & newVal);
    void setUnexpectedKeywordText          (const std::string          & newVal);

    void setCommentMarker                  (char                         newVal);
    void setMultilineMarker                (char                         newVal);
    void setVerbose                        (bool                         newVal);

    //! @throws std::runtime_error via descriptorValidityCheck
    void addKeyword                  (const             Parrot::Descriptor  & descriptor );
    //! @throws std::runtime_error via descriptorValidityCheck
    void addKeywords                 (const std::vector<Parrot::Descriptor> & descriptors);


    /**
     * @throws std::runtime_error if non-matching vector lengths or via descriptorValidityCheck
     */
    void addKeywords                 (const std::vector<std::string> &              keywords,
                                      const std::vector<Parrot::ValueTypeID> &      valueTypes,
                                      const std::vector<bool> &                     mandatory = std::vector<bool>()
    );
    /**
     * @throws std::runtime_error if non-matching vector lengths or via descriptorValidityCheck
     */
    void addKeywords                 (const std::vector<std::string> &              keywords,
                                      const std::vector<std::any> &                 defaultValues,
                                      const std::vector<bool> &                     mandatory = std::vector<bool>()
    );

    //! @throws std::runtime_error via descriptorValidityCheck
    void addKeyword                  (const std::string &                           keyword,
                                      ValueTypeID                                   valueType,
                                      bool                                          mandatory                 = true
    );
    //! @throws std::runtime_error via descriptorValidityCheck
    template <typename T>
    void addKeyword                  (const std::string &                           keyword,
                                      const T &                                     defaultValue,
                                      bool                                          mandatory                 = true
    );

    //! @throws std::runtime_error via descriptorValidityCheck
    void addKeywordRanged            (const std::string &                           keyword,
                                      ValueTypeID                                   valueType,
                                      PARROT_TYPE(ValueTypeID::Real)                min,
                                      PARROT_TYPE(ValueTypeID::Real)                max,
                                      RestrictionViolationPolicy                    policy                    = RestrictionViolationPolicy::Exception,
                                      const std::string &                           restrictionViolationText  = "value out of bounds",
                                      bool                                          mandatory                 = true
    );
    //! @throws std::runtime_error via descriptorValidityCheck
    template <typename T>
    void addKeywordRanged            (const std::string &                           keyword,
                                      const T &                                     defaultValue,
                                      PARROT_TYPE(ValueTypeID::Real)                min,
                                      PARROT_TYPE(ValueTypeID::Real)                max,
                                      RestrictionViolationPolicy                    policy                    = RestrictionViolationPolicy::Exception,
                                      const std::string &                           restrictionViolationText  = "value out of bounds",
                                      bool                                          mandatory                 = true
    );

    //! @throws std::runtime_error via descriptorValidityCheck
    void addKeywordListboundPreParse (const std::string &                           keyword,
                                      ValueTypeID                                   valueType,
                                      const PARROT_TYPE(ValueTypeID::StringList) &  list,
                                      bool                                          forbiddenList             = false,
                                      RestrictionViolationPolicy                    policy                    = RestrictionViolationPolicy::Exception,
                                      const std::string &                           restrictionViolationText  = "value out of bounds",
                                      bool                                          mandatory                 = true
    );
    //! @throws std::runtime_error via descriptorValidityCheck
    template <typename T>
    void addKeywordListboundPreParse (const std::string &                           keyword,
                                      const T &                                     defaultValue,
                                      const PARROT_TYPE(ValueTypeID::StringList) &  list,
                                      bool                                          forbiddenList             = false,
                                      RestrictionViolationPolicy                    policy                    = RestrictionViolationPolicy::Exception,
                                      const std::string &                           restrictionViolationText  = "value out of bounds",
                                      bool                                          mandatory                 = true
    );

    //! @throws std::runtime_error via descriptorValidityCheck
    template <typename LT>
    void addKeywordListboundAftParse (const std::string &                           keyword,
                                      ValueTypeID                                   valueType,
                                      const std::vector<LT> &                       list,
                                      bool                                          forbiddenList             = false,
                                      RestrictionViolationPolicy                    policy                    = RestrictionViolationPolicy::Exception,
                                      const std::string &                           restrictionViolationText  = "value out of bounds",
                                      bool                                          mandatory                 = true
    );
    //! @throws std::runtime_error via descriptorValidityCheck
    template <typename DT, typename LT>
    void addKeywordListboundAftParse (const std::string &                           keyword,
                                      const DT &                                    defaultValue,
                                      const std::vector<LT> &                       list,
                                      bool                                          forbiddenList             = false,
                                      RestrictionViolationPolicy                    policy                    = RestrictionViolationPolicy::Exception,
                                      const std::string &                           restrictionViolationText  = "value out of bounds",
                                      bool                                          mandatory                 = true
    );

    //! @throws std::runtime_error via descriptorValidityCheck
    void addKeywordUserboundPreParse (const std::string &                           keyword,
                                      ValueTypeID                                   valueType,
                                      const std::function< bool(const PARROT_TYPE(ValueTypeID::String) &)> &uFunc,
                                      RestrictionViolationPolicy                    policy                    = RestrictionViolationPolicy::Exception,
                                      const std::string &                           restrictionViolationText  = "value out of bounds",
                                      bool                                          mandatory                 = true
    );
    //! @throws std::runtime_error via descriptorValidityCheck
    template <typename T>
    void addKeywordUserboundPreParse (const std::string &                           keyword,
                                      const T &                                     defaultValue,
                                      const std::function< bool(const PARROT_TYPE(ValueTypeID::String) &)> &uFunc,
                                      RestrictionViolationPolicy                    policy                    = RestrictionViolationPolicy::Exception,
                                      const std::string &                           restrictionViolationText  = "value out of bounds",
                                      bool                                          mandatory                 = true
    );

    //! @throws std::runtime_error via descriptorValidityCheck
    template <typename T>
    void addKeywordUserboundAftParse (const std::string &                           keyword,
                                      ValueTypeID                                   valueType,
                                      const std::function<bool (const T &)> &       uFunc,
                                      RestrictionViolationPolicy                    policy                    = RestrictionViolationPolicy::Exception,
                                      const std::string &                           restrictionViolationText  = "value out of bounds",
                                      bool                                          mandatory                 = true
    );
    //! @throws std::runtime_error via descriptorValidityCheck
    template <typename T>
    void addKeywordUserboundAftParse (const std::string &                           keyword,
                                      const T &                                     defaultValue,
                                      const std::function<bool (const T &)> &       uFunc,
                                      RestrictionViolationPolicy                    policy                    = RestrictionViolationPolicy::Exception,
                                      const std::string &                           restrictionViolationText  = "value out of bounds",
                                      bool                                          mandatory                 = true
    );

    // ---------------------------------------------------------------------- //
    // I/O

    FileContent operator() (const std::string & source) const;

    std::string to_string() const;
  };
}

// ========================================================================== //
// template implementations

#include "Parrot/Reader.tpp"

// ========================================================================== //

#endif
