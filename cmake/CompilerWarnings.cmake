function(set_project_warnings project_name)
  option(NDR_WARNINGS_AS_ERROR "Treat Compiler warnings as errors" TRUE)

  # I don't currently have access to a windows machine to test these,
  # so rather than trial and error pushing to trigger appveyor and check
  # for errors, leave it at this minimal list.
  set(MSVC_WARNINGS
    /permissive-
    /W4
    )

  set(CLANG_WARNINGS
    -Wall
    -Wextra
    -Wpedantic
    #-Wshadow # {fmt} uses -Wno-shadow so breaks CI. I want CI to be green
    -Wnon-virtual-dtor
    -Woverloaded-virtual
    -Wnull-dereference
    -Wold-style-cast
    -Wcast-align
    -Wfloat-equal
    -Wunused
    #-Wconversion # STL containers uses size_t so this gives lots of warnings
    #-Wsign-conversion # STL containers uses size_t so this gives lots of warnings
    )

  if (WARNINGS_AS_ERRORS)
    set(CLANG_WARNINGS ${CLANG_WARNINGS} -Werror)
    set(MSVC_WARNINGS ${MSVC_WARNINGS} /WX)
  endif()

  set(GCC_WARNINGS
    ${CLANG_WARNINGS}
    -Wmisleading-indentation
    -Wduplicated-branches
    -Wduplicated-cond
    -Wlogical-op
    -Wuseless-cast
    )

  if(MSVC)
    set(PROJECT_WARNINGS ${MSVC_WARNINGS})
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(PROJECT_WARNINGS ${CLANG_WARNINGS})
  else()
    set(PROJECT_WARNINGS ${GCC_WARNINGS})
  endif()

  target_compile_options(${project_name} INTERFACE ${PROJECT_WARNINGS})

endfunction(set_project_warnings)
