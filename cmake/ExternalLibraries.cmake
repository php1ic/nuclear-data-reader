function(add_external_libraries project_name)
  # Use the {fmt} library. It will be part of C++20
  # clang needs the -fPIC when linking so force it
  # GCC appears to work without it
  set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
  add_subdirectory(external/fmt EXCLUDE_FROM_ALL)
  # Using fmt::output_file requires the compiled library. We can't use header only
  target_link_libraries(project_options INTERFACE fmt::fmt)
endfunction(add_external_libraries)
