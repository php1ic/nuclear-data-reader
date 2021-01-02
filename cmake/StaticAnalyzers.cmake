option(NDR_ENABLE_CPPCHECK "Enable static analysis with cppcheck" OFF)
if(NDR_ENABLE_CPPCHECK)
  find_program(CPPCHECK cppcheck)
  if(CPPCHECK)
    set(CMAKE_CXX_CPPCHECK
        ${CPPCHECK}
        --enable=all
        --std=c++17
        --template="[{severity}][{id}] {message} {callstack} \(On {file}:{line}\)"
        --suppress=missingInclude
        --inline-suppr
        --inconclusive
        -i ${PROJECT_SOURCE_DIR}/tests
        -I ${PROJECT_SOURCE_DIR}/include
        -I ${PROJECT_SOURCE_DIR}/external/Catch2/single_include
        -I ${PROJECT_SOURCE_DIR}/external/fmt/include
        )
      message(STATUS "[cppcheck] Configuring to use the binary ${CMAKE_CXX_CPPCHECK}")
  else()
    message(SEND_ERROR "[cppcheck] Requested but executable not found")
  endif()
else()
  message(STATUS "[cppcheck] Not running cppcheck as part of the build step")
endif()

option(NDR_ENABLE_CLANG_TIDY "Enable static analysis with clang-tidy" OFF)
if(NDR_ENABLE_CLANG_TIDY)
  # No need for options, there is a config file at the root
  find_program(CLANGTIDY clang-tidy)
  if(CLANGTIDY)
    set(CMAKE_CXX_CLANG_TIDY ${CLANGTIDY})
    message(STATUS "[clang-tidy] Configuring to use the binary: ${CMAKE_CXX_CLANG_TIDY}")
  else()
    message(SEND_ERROR "[clang-tidy] Requested but executable not found")
  endif()
else()
  message(STATUS "[clang-tidy] Not running clang-tidy as part of the build step")
endif()
