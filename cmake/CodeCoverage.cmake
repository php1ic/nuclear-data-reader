function(enable_codecoverage project_name)
  option(NDR_CODE_COVERAGE "Enable coverage reporting" OFF)

  if(NDR_CODE_COVERAGE)
    add_library(coverage_config INTERFACE)

    if(NDR_CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
      message(STATUS "[code-coverage] Measure coverage metrics")

      # Add required flags (GCC & LLVM/Clang)
      target_compile_options(coverage_config
        INTERFACE
        -O0        # no optimization
        -g         # generate debug info
        --coverage # sets all required flags
        )

      target_link_options(coverage_config INTERFACE --coverage)
    endif()

    target_link_libraries(${project_name} INTERFACE coverage_config)
  else()
    message(STATUS "[code-coverage] Not measuring metrics")
  endif()

endfunction(enable_codecoverage)
