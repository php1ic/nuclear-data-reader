# Locate all of the header files so IDEs can find them
# Keep list alphabetical
set(HEADERS
  ame_data.hpp
  ame_mass_position.hpp
  ame_reaction1_position.hpp
  ame_reaction2_position.hpp
  converter.hpp
  isotope.hpp
  massTable.hpp
  nubase_data.hpp
  nubase_line_position.hpp
  number.hpp
  version.hpp
  )


configure_file(
  "version.hpp.in"
  "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/version.hpp"
  @ONLY
  )
