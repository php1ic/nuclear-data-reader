function(enable_sanitizers project_name)

  if(NOT MSVC)
    set(SANITIZERS "")

    option(NDR_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" FALSE)
    if(NDR_ENABLE_SANITIZER_ADDRESS)
      list(APPEND SANITIZERS "address")
    endif()

    # memory sanitizser currently only for clang
    option(NDR_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" FALSE)
    if(NDR_ENABLE_SANITIZER_MEMORY AND CMAKE_CXX_COMPILERID STREQUAL "Clang")
      list(APPEND SANITIZERS "memory")
    endif()

    option(NDR_ENABLE_SANITIZER_UNDEFINED_BEHAVIOR "Enable undefined behavior sanitizer" FALSE)
    if(NDR_ENABLE_SANITIZER_UNDEFINED_BEHAVIOR)
      list(APPEND SANITIZERS "undefined")
    endif()

    option(NDR_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" FALSE)
    if(NDR_ENABLE_SANITIZER_THREAD)
      list(APPEND SANITIZERS "thread")
    endif()

    list(JOIN SANITIZERS "," LIST_OF_SANITIZERS)
  endif()

  if(NOT "${LIST_OF_SANITIZERS}" STREQUAL "")
    target_compile_options(
      ${project_name}
      INTERFACE
      -fsanitize=${LIST_OF_SANITIZERS}
      )

    target_link_libraries(
      ${project_name}
      INTERFACE
      -fsanitize=${LIST_OF_SANITIZERS}
      )
    message(STATUS "[sanitizers] Using: ${LIST_OF_SANITIZERS}")
  else()
    message(STATUS "[sanitizers] Not using any sanitizers")
  endif()

endfunction()
