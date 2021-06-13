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
 * @todo finish!
 * @}
 */

#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include "Parrot/Definitions.hpp"
#include "Parrot/Restriction.hpp"
#include "Parrot/Descriptor.hpp"
#include "Parrot/Reader.hpp"

#endif
