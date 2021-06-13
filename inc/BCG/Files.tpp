// ========================================================================== //
// local macro

#define THROWTEXT(msg) (std::string("RUNTIME EXCEPTION IN ") + (__PRETTY_FUNCTION__) + "\n" + msg)

// ========================================================================== //
// procs

static inline std::fstream BCG::openThrow(const std::string & filename, std::ios_base::openmode mode) {
  auto reVal = std::fstream(filename, mode);

  if ( !reVal.is_open() ) {
    throw( std::invalid_argument("failed to open '" + filename + "'") );
  }

  return reVal;
}
// ........................................................................ //
static inline std::string  BCG::generateFileComments(const std::string & content) {
  std::string reVal;

  reVal += "# ============================================================================ #\n";
  auto lines = BCG::splitString(content, '\n');
  for (const auto & line : lines) {reVal += "# " + line + "\n";}
  reVal += "# timestamp: ";
  reVal +=    generateTimestamp() + "\n";
  reVal += "# ============================================================================ #\n\n";

  return reVal;
}

// ========================================================================== //

#undef THROWTEXT



