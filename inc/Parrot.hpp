/**
 * @defgroup Parrot_Main Parrot File Parser
 *
 * @brief A C++ Text File Parser
 *
 * @{
 * @page Parrot_About
 * @}
 */

/**
 * @page Parrot_About Parrot Structure
 *
 * @{
 * \em Parrot is built around the \c Parrot::Reader class. This object
 * interprets a text file according to a number of \c Parrot::Descriptor
 * instances. Each such instance describes the handling of one keyword in the
 * text file. Such Descriptor may also comprise of several instances of
 * \c Parrot::Restriction, representing the range of valid values to the keyword
 * as well as the event that is triggered when a read value does not comply with
 * these restrictions.<br>
 * A \c Parrot::Reader can be invoked with a filename as a parameter. If
 * successful, this will return a \c Parrot::Filecontent object which contains
 * the interpreted content of the file.
 *
 * All symbols and functions of the \em Parrot module are defined in the
 * \c Parrot namespace.
 *
 * See these class modules for details on the package:
 * * @subpage Parrot_Definitions "\c Parrot::Definitions"
 *    -- symbols used in the entire package and convenience functions
 * * \c Parrot::Restriction -- represents the restrictions applied to a value
 *    for a given keyword as well as what event should be triggered if the value
 *    does not meet these restrictions
 * * \c Parrot::Descriptor -- represents a keyword together with rules for
 *    parsing the corresponding file
 * * \c Parrot::Reader -- a callable instance that reads a \c .ini file and
 *    returns a \c Parrot::Filecontent object.
 * * \c Parrot::Filecontent -- the parsed content of a file, together with state
 *    variables indicating missing or malformed expressions.
 *
 * @todo quick guide to using Parrot
 * @todo link to the quick guide from main
 * @todo finish!
 * @}
 */

/**
 * @page Parrot_QuickGuide A Quick Guide to Parrot
 *
 * @todo make quick guide
 */

#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include "Parrot/Definitions.hpp"
#include "Parrot/Restriction.hpp"
#include "Parrot/Descriptor.hpp"
#include "Parrot/Reader.hpp"

#endif
