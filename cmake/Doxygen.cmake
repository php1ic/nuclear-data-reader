function(enable_doxygen)
  option(NDR_BUILD_DOCS "Build documentation with Doxygen" OFF)
  if(NDR_BUILD_DOCS)
    find_package(Doxygen)

    if(NOT DOXYGEN_FOUND)
      message(SEND_ERROR "[doxygen] Requested but executable not found.")
      return()
    endif()

    # Don't document external libraries
    set(DOXYGEN_EXCLUDE_PATTERNS "*/external/*")

    message(STATUS "[doxygen ${DOXYGEN_VERSION}] Building documentation")

    # We could exclude this target from 'all',
    # but if the user has turned this option on lets assume they want it.
    doxygen_add_docs(doxygen_docs
      WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
      COMMENT "[doxygen] Generating documentation"
      )
  else()
    message(STATUS "[doxygen] Not building documentation")
  endif()
endfunction(enable_doxygen)
