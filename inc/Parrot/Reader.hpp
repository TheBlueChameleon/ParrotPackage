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
#include <tuple>

// own
#include "Parrot/Definitions.hpp"
#include "Parrot/Descriptor.hpp"
#include "Parrot/FileContent.hpp"

// ========================================================================== //

namespace Parrot {

  // ======================================================================== //
  // class

  /**
   * @brief A callabe object, capable of parsing a .ini file and returning a
   *    \c Parrot::FileContent() object
   *
   * From user point of view, the Reader is the principal component of the
   *    *Parrot* package. It stores a collection of <tt>Parrot::Descriptor</tt>s
   *    that contain all parsing rules on keyword level as well as globally
   *    applied rules.
   *
   * Calling the object with a filename argument reads and parses the file and
   *    returns a \c Parrot::FileContent object unless the file is not
   *    accessible or the violation of a \c Parrot::Restriction causes an error
   *    to be thrown.
   *
   */
  class Reader {
  public:
    /**
     * @brief a std::tuple of a keyword string, a Parrot::ValueTypeID and a
     *    boolean marking the keyword as mandatory.
     */
    using MinimalDescriptor = std::tuple<std::string, Parrot::ValueTypeID, bool>;

  private:
    // ...................................................................... //
    // state variables

    char                            commentMarker                   = '#';
    char                            multilineMarker                 = '\\';
    char                            assignmentMarker                = '=';
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

    /* *text variables
     * $F -- file               : filename as specified in call operator (via content)
     * $L -- line               : line as read from file, trimmed        (lineOriginal)
     * $# -- linenumber         : current line number in parsing process (linenumber)
     * $K -- keyword            : current keyword                        (keyword)
     * $D -- default value      : default value of given keyword         (defaultValue)
     * $V -- value (read value) : parsed value as string                 (readValue)
     */

    std::string       lineOriginal   ;
    std::string       linePreparsed  ;
    std::string       keyword        ;
    std::string       defaultValue   ;
    std::string       readValue      ;
    int               linenumber = -1;
    std::vector<bool> foundInFile    ;
    FileContent       content        ;

    // ...................................................................... //
    // parsing machinery

    void descriptorValidityCheck(const Parrot::Descriptor & descriptor) const;
    void parseLine();

  public:
    // ---------------------------------------------------------------------- //
    // CTors

    Reader() = default;
    /**
     * @brief copies all <tt>Parrot::Descriptor</tt>s from \c descriptor into
     *    the parsing ruleset for later application.
     */
    Reader(const std::vector<Parrot::Descriptor> & descriptors);

    // ---------------------------------------------------------------------- //
    // Getters

    //! returns the character indicating a comment line
    char                                    getCommentMarker    () const;
    //! returns the character indicating a continued line
    char                                    getMultilineMarker  () const;
    //! returns the character separating keyword from value
    char                                    getAssignmentMarker () const;
    //! returns whether or not the parsing progress should be printed to stdout
    bool                                    getVerbose          () const;

    //! returns the event triggered if a mandatory keyword was not found in file
    const MissingKeywordPolicy &            getMissingKeywordPolicyMandatory  () const;
    //! returns the text output for when a mandatory keyword was not found in file
    const std::string          &            getMissingKeywordTextMandatory    () const;
    //! returns the event triggered if a non-mandatory keyword was not found in file
    const MissingKeywordPolicy &            getMissingKeywordPoliyNonMandatory() const;
    //! returns the text output for when a non-mandatory keyword was not found in file
    const std::string          &            getMissingKeywordTextNonMandatory () const;
    //! returns the event triggered if a keyword without descriptor was found in file
    const MissingKeywordPolicy &            getUnexpectedKeywordPolicy        () const;
    //! returns the text output for when a keyword without descriptor was found in file
    const std::string          &            getUnexpectedKeywordText          () const;

    //! returns the number of currently registered keywords
    size_t                                  size      () const;
    //! returns true if a given keyword has already been registered
    bool                                    hasKeyword(const std::string & keyword) const;

    //! returns all currently registered keywords
    const std::vector<Parrot::Descriptor> & getDescriptors() const;
    /**
     * Returns the <tt>idx</tt><sup>th</sup> descriptor registered (in
     *    chronological order), or throws an error if the index is not in the
     *    valid range
     *
     * @param index the zero-based index of the \c Parrot::Descriptor to return
     *
     * @throws std::out_of_range if index out of bounds
     */
    const             Parrot::Descriptor  & getDescriptor (const size_t        index  ) const;
    /**
     * Returns the descriptor assosicated with the keyword \c keyword, if it has
     *    been registered so far. Otherwise throws an error
     *
     * @param keyword the key of the \c Parrot::Descriptor to return
     *
     * @throws std::out_of_range if no \c Parrot::Descriptor with key
     *    \c keyword has been registered to the \c Parrot::Reader
     */
    const             Parrot::Descriptor  & getDescriptor (const std::string & keyword) const;

    // ---------------------------------------------------------------------- //
    // Setters

    //! restores the state as if generated by the empty CTor
    void reset();
    //! removes all registered descriptors but preserved globally applied attributes
    void resetKeywords();

    //! sets the kind of event triggered when a mandatory keyword is not found in a file
    void setMissingKeywordPolicyMandatory   (const MissingKeywordPolicy & newVal);
    //! sets the text output for when a mandatory keyword is not found in a file
    void setMissingKeywordTextMandatory     (const std::string          & newVal);
    //! sets the kind of event triggered when a non-mandatory keyword is not found in a file
    void setMissingKeywordPoliyNonMandatory (const MissingKeywordPolicy & newVal);
    //! sets the text output for when a non-mandatory keyword is not found in a file
    void setMissingKeywordTextNonMandatory  (const std::string          & newVal);
    //! sets the kind of event triggered when a keyword is found in a file for which no descriptor exists
    void setUnexpectedKeywordPolicy         (const MissingKeywordPolicy & newVal);
    //! sets the text output for when a keyword is found in a file for which no descriptor exists
    void setUnexpectedKeywordText           (const std::string          & newVal);

    /**
     * @brief sets the character that introduces a comment line
     *
     * *Parrot* supports only full-line-comments, i.e. a comment marker to the
     *    right of an assignment character (e.g. '=') not treated as comment
     *    markers but rather as part of the keyword value.
     *
     * Leading whitespaces are ignored when checking for comment lines
     *
     * @note If you want to deactivate comments, set this to the null char (<tt>\\0</tt>)
     *
     * Example:
     @verbatim
     # this is a regular comment
         # this, too, will be treated as a comment
     keyword = some string # that contains the comment marker
     @endverbatim
     *
     */
    void setCommentMarker                   (char                         newVal);
    /**
     * @brief sets the character that indicates the current keyword is multiline
     *
     * If this character is the last one in a line (before the line break
     *    character(s)), then the subsequent line will be added to the former,
     *    after trimming whitespaces.
     *
     * Whitespaces after the line continuation marker are trimmed before
     *    considering line continuation. Whitespaces before the continuation
     *    marker are \e never trimmed.
     *
     * @note If you want to deactivate multiline keywords, set this to the null
     *    char (<tt>\\0</tt>)
     *
     * Example:
     @verbatim
     multiline keyword = this is a \
          long line which was split into mult\
          iple lines
     @endverbatim
     * is equivalent to
     @verbatim
     multiline keyword = this is a long line which was split into multiple lines
     @endverbatim
     */
    void setMultilineMarker                 (char                         newVal);
    /**
     * @brief sets the character that separates keyword from keyword value
     *
     * Whitespace trimming is handled separately for the keyword (i.e. the
     *    portion to the left of the assignment character) and the value (i.e.
     *    the portion to the right)
     *
     * The keyword is always both, trimmed from leading and trailing
     *    whitespaces. The value obeys the rules set in the according
     *    descriptor.
     *
     */
    void setAssignmentMarker                (char                         newVal);
    //! activates or deactivates debug parsing output
    void setVerbose                         (bool                         newVal);


    /**
     * Adds a \c Parrot::Descriptor to the list of expected keywords
     *
     * @param descriptor a \c Parrot::Descriptor with arbitrary content
     *
     * @throws Parrot::InvalidDescriptorError if a \c Parrot::Descriptor with
     *    the same keyword was previously registered or if the keyword string is
     *    empty
     */
    void addKeyword                  (const             Parrot::Descriptor &        descriptor);
    /**
     * Builds a \c Parrot::Descriptor from the minimal set of required
     *    information and adds it to the list of expected keywords
     *
     * @param descriptor a \c Parrot::Reader::MinimalDescriptor, i.e. a
     *    <tt>std::tuple&lt;std::string, Parrot::ValueTypeID, bool&gt;</tt>
     *    (keyword name, keyword value type and whether it is a mandatory
     *    keyword) providing the information necessary to build a
     *    \c Parrot::Descriptor.
     *
     * @throws Parrot::InvalidDescriptorError if a \c Parrot::Descriptor with
     *    the same keyword was previously registered or if the keyword string is
     *    empty
     */
    void addKeyword                  (const Parrot::Reader::MinimalDescriptor &     descriptor);
    /**
     * Builds a \c Parrot::Descriptor from the minimal set of required
     *    information and adds it to the list of expected keywords
     *
     * @param keyword the keyword name by which the corresponding part of the
     *    file will be recognized
     * @param valueType the associated data type in which to cast the keyword
     *    value
     * @param mandatory whether or not to treat the keyword as mandatory.
     *
     * @throws Parrot::InvalidDescriptorError if a \c Parrot::Descriptor with
     *    the same keyword was previously registered or if the keyword string is
     *    empty
     */
    void addKeyword                  (const std::string &                           keyword,
                                      Parrot::ValueTypeID                           valueType,
                                      bool                                          mandatory                 = true
    );
    /**
     * Builds a \c Parrot::Descriptor from the minimal set of required
     *    information and adds it to the list of expected keywords
     *
     * @param keyword the keyword name by which the corresponding part of the
     *    file will be recognized
     * @param defaultValue the value to use as default if the keyword is not
     *    found in the file. Note that this value must be explicitly of one of
     *    the types specified in \c Parrot::ValueTypeID().
     * @param mandatory whether or not to treat the keyword as mandatory.
     *
     * @throws Parrot::InvalidDescriptorError if a \c Parrot::Descriptor with
     *    the same keyword was previously registered or if the keyword string is
     *    empty
     */
    void addKeyword                  (const std::string &                           keyword,
                                      std::any &                                    defaultValue,
                                      bool                                          mandatory                 = false
    );


    //! @throws Parrot::InvalidDescriptorError via descriptorValidityCheck
    void addKeywords                 (const std::vector<Parrot::Descriptor> &       descriptors);
    //! @throws Parrot::InvalidDescriptorError via descriptorValidityCheck
    void addKeywords                 (const std::vector<MinimalDescriptor> &        descriptors);
    /**
     * @throws std::logic_error if non-matching vector lengths or via descriptorValidityCheck
     */
    void addKeywords                 (const std::vector<std::string> &              keywords,
                                      const std::vector<Parrot::ValueTypeID> &      valueTypes,
                                      const std::vector<bool> &                     mandatory = std::vector<bool>()
    );
    /**
     * @throws std::logic_error if non-matching vector lengths or via descriptorValidityCheck
     */
    void addKeywords                 (const std::vector<std::string> &              keywords,
                                      const std::vector<std::any> &                 defaultValues,
                                      const std::vector<bool> &                     mandatory = std::vector<bool>()
    );


    //! @throws Parrot::InvalidDescriptorError via descriptorValidityCheck
    template <typename T>
    void addKeyword                  (const std::string &                           keyword,
                                      const T &                                     defaultValue,
                                      bool                                          mandatory                 = true
    );

    //! @throws Parrot::InvalidDescriptorError via descriptorValidityCheck
    void addKeywordRanged            (const std::string &                           keyword,
                                      Parrot::ValueTypeID                           valueType,
                                      PARROT_TYPE(ValueTypeID::Real)                min,
                                      PARROT_TYPE(ValueTypeID::Real)                max,
                                      RestrictionViolationPolicy                    policy                    = RestrictionViolationPolicy::Exception,
                                      const std::string &                           restrictionViolationText  = "value out of bounds",
                                      bool                                          mandatory                 = true
    );
    //! @throws Parrot::InvalidDescriptorError via descriptorValidityCheck
    template <typename T>
    void addKeywordRanged            (const std::string &                           keyword,
                                      const T &                                     defaultValue,
                                      PARROT_TYPE(ValueTypeID::Real)                min,
                                      PARROT_TYPE(ValueTypeID::Real)                max,
                                      RestrictionViolationPolicy                    policy                    = RestrictionViolationPolicy::Exception,
                                      const std::string &                           restrictionViolationText  = "value out of bounds",
                                      bool                                          mandatory                 = true
    );

    //! @throws Parrot::InvalidDescriptorError via descriptorValidityCheck
    void addKeywordListboundPreParse (const std::string &                           keyword,
                                      Parrot::ValueTypeID                           valueType,
                                      const PARROT_TYPE(ValueTypeID::StringList) &  list,
                                      bool                                          forbiddenList             = false,
                                      RestrictionViolationPolicy                    policy                    = RestrictionViolationPolicy::Exception,
                                      const std::string &                           restrictionViolationText  = "value out of bounds",
                                      bool                                          mandatory                 = true
    );
    //! @throws Parrot::InvalidDescriptorError via descriptorValidityCheck
    template <typename T>
    void addKeywordListboundPreParse (const std::string &                           keyword,
                                      const T &                                     defaultValue,
                                      const PARROT_TYPE(ValueTypeID::StringList) &  list,
                                      bool                                          forbiddenList             = false,
                                      RestrictionViolationPolicy                    policy                    = RestrictionViolationPolicy::Exception,
                                      const std::string &                           restrictionViolationText  = "value out of bounds",
                                      bool                                          mandatory                 = true
    );

    //! @throws Parrot::InvalidDescriptorError via descriptorValidityCheck
    template <typename LT>
    void addKeywordListboundAftParse (const std::string &                           keyword,
                                      Parrot::ValueTypeID                           valueType,
                                      const std::vector<LT> &                       list,
                                      bool                                          forbiddenList             = false,
                                      RestrictionViolationPolicy                    policy                    = RestrictionViolationPolicy::Exception,
                                      const std::string &                           restrictionViolationText  = "value out of bounds",
                                      bool                                          mandatory                 = true
    );
    //! @throws Parrot::InvalidDescriptorError via descriptorValidityCheck
    template <typename DT, typename LT>
    void addKeywordListboundAftParse (const std::string &                           keyword,
                                      const DT &                                    defaultValue,
                                      const std::vector<LT> &                       list,
                                      bool                                          forbiddenList             = false,
                                      RestrictionViolationPolicy                    policy                    = RestrictionViolationPolicy::Exception,
                                      const std::string &                           restrictionViolationText  = "value out of bounds",
                                      bool                                          mandatory                 = true
    );

    //! @throws Parrot::InvalidDescriptorError via descriptorValidityCheck
    void addKeywordUserboundPreParse (const std::string &                           keyword,
                                      Parrot::ValueTypeID                           valueType,
                                      const std::function< bool(const PARROT_TYPE(ValueTypeID::String) &)> &uFunc,
                                      RestrictionViolationPolicy                    policy                    = RestrictionViolationPolicy::Exception,
                                      const std::string &                           restrictionViolationText  = "value out of bounds",
                                      bool                                          mandatory                 = true
    );
    //! @throws Parrot::InvalidDescriptorError via descriptorValidityCheck
    template <typename T>
    void addKeywordUserboundPreParse (const std::string &                           keyword,
                                      const T &                                     defaultValue,
                                      const std::function< bool(const PARROT_TYPE(ValueTypeID::String) &)> &uFunc,
                                      RestrictionViolationPolicy                    policy                    = RestrictionViolationPolicy::Exception,
                                      const std::string &                           restrictionViolationText  = "value out of bounds",
                                      bool                                          mandatory                 = true
    );

    //! @throws Parrot::InvalidDescriptorError via descriptorValidityCheck
    template <typename T>
    void addKeywordUserboundAftParse (const std::string &                           keyword,
                                      Parrot::ValueTypeID                           valueType,
                                      const std::function<bool (const T &)> &       uFunc,
                                      RestrictionViolationPolicy                    policy                    = RestrictionViolationPolicy::Exception,
                                      const std::string &                           restrictionViolationText  = "value out of bounds",
                                      bool                                          mandatory                 = true
    );
    //! @throws Parrot::InvalidDescriptorError via descriptorValidityCheck
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

    Parrot::FileContent operator() (const std::string & source);

    std::string to_string() const;
  };
}

// ========================================================================== //
// template implementations

#include "Parrot/Reader.tpp"

// ========================================================================== //

#endif
