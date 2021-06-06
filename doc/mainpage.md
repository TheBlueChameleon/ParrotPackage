*%Parrot* is a package that allows parsing of ```.ini``` files and checking them for pre- and userdefined validation rules. A ```.ini``` is understood to be a human readable text file that is comprised of key-value pair, separated by a designated character. By default, this character is an equals sign (```=```). Further, comment lines (i.e. lines that should be ignored by the computer) may be part of the ```.ini``` file as well. By default, comment lines begin with a pound symbol (```#```).

A valid input for *%Parrot* thus could look like this:

---

<pre>
# PRA %Settings file
N_modes   = 2
N_photons = 5
threads   = 4

# possible values for keyword_occupation:
#   NONZERO, EQUAL, ALL
keyword_occupation      = ALL
parameter_occupation    = NONE
keyword_initial_guess   = SWEEP
parameter_initial_guess = 0:2PI:100; 0:2PI:100, 0

epsilon_convergence = 1E-6
epsilon_backtest    = 1E-3
epsilon_unique      = 1E-1
max_iterations      = 300

# possible values for mission:
#   FINDUNIQUE,
#   MAKEPHASELIST, MAKEVECTORLIST,
#   MAKEITERMAP, MAKEIDMAP, MAKECONVERGENCEMAP, MAKESUCCESSMAP,
#   MAKEERRORCOUNTMAP, MAKESOLUTIONCOUNTMAP, MAKEEFFORTMAP
mission         = FINDUNIQUE, MAKEVECTORLIST, MAKEITERMAP, MAKEIDMAP, MAKEERRORCOUNTMAP
outputformat    = TXT, GNUPLOT, PDF
plotdimensions  = AUTO
plotslice       = AUTO

filename_directory = ./output/
filename_prefix = sweep2D_K=2_N=2_
filename_midfix = EXPLICIT
filename_suffix =
</pre>

.
---


While easy to read and write for a human being, this example ```.ini``` file already shows quite some complexity when it comes to reading it into memory and correctly interpreting it. There are different data types (integers, real numbers, strings, booleans and lists thereof), a variable number of whitespaces and line breaks in between, the order of the lines is not necessarily fixed, some values may be missing or optional, and some givens may be unuseable for a given problem.

# Nomenclature
Throughout this document, the following notation will be used:

* A **keyword** is whatever string can be found on the left hand side of the *separator character*. In the above example, ```keyword_occupation```
    or ```max_iterations``` would be keywords.
* A **value** is what is on the right hand side of the *separator character*. Value from the above example are, amoungst others, ```ALL``` or ```300```.
* The **separator character** is a single character that separates *keywords* and *values*. In the above, this separator character is the equals sign (```=```)
* The **comment character** is a single character that marks one line as a comment for human readers. In the above example, this is the pound symbol (```#```)
* Values have an associated **Value Type**. This value type is not deducible from the ```.ini``` file (albeit often strongly implied) alone but needs to be specified
  by a *%Descriptor*. It is one of the following concepts:
    * A **String** is an arbitrary sequence of characters barring a line break
    * An **Integer** is a whole number including negative numbers like 1 or -58008
    * A **Real Value** is a possibly fractional or irrational numerical value like 1.0 or -3.141592654
    * A **Boolean** is binary information, represented by either of the values ```TRUE```, ```YES```, ```ON``` or ```FALSE```, ```NO```, ```OFF```.
        The first three of these values are interpreted to mean the same, as do the last three of them.
    * Lists thereof are composition of several values of the stame value type, separated by a designated character. Usually, this *list separator*
        is a comma (```,```). Each list can have its own list separator.
* A **list separator** is a single character that is used to tell apart the different constituents of a list.

# Modules
To faciliate the process of reading and parsing such ```.ini```  files, the *%Parrot* package was conceived. Its primary element is the Settings class, which first can be fed with specifications about the file to read and then instructed to perform the desired parsing with minimal effort.

