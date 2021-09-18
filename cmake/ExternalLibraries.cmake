function(add_external_libraries project_name)
  # Use the {fmt} library. It will be part of C++20
  add_subdirectory(external/fmt EXCLUDE_FROM_ALL)
  # Using fmt::output_file requires the compiled library. We can't use header only
  target_link_libraries(project_options INTERFACE fmt::fmt)

  # Need to link the filesystem library in older compiler versions
  # But not for MSVC
  target_link_libraries(project_options INTERFACE $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:stdc++fs>)
endfunction(add_external_libraries)
