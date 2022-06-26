# The project knows Catch2 exits, but include this file to access catch_discover_tests()
include(${PROJECT_SOURCE_DIR}/external/Catch2/extras/Catch.cmake)

# Print the version of catch for info
get_directory_property(
  CATCH_VERSION
  DIRECTORY "${PROJECT_SOURCE_DIR}/external/Catch2"
  DEFINITION PROJECT_VERSION
  )

message(STATUS "Catch2 version: ${CATCH_VERSION}")

# Give the full test executable a name (not the project name)
set(NDR_TEST_NAME UnitTests)

# Alphabetical list of all the test source files
set(TEST_SOURCES
  ame_data_test.cpp
  converter_test.cpp
  isotope_test.cpp
  massTable_test.cpp
  nubase_data_test.cpp
  )

# Create the tests
add_executable(${NDR_TEST_NAME} ${TEST_SOURCES})

# Where are the header files
target_include_directories(${NDR_TEST_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include/)

target_link_libraries(
  ${NDR_TEST_NAME}
  PRIVATE
  ${PROJECT_NAME}
  Catch2::Catch2WithMain
  project_warnings
  project_options
  )

catch_discover_tests(
  ${NDR_TEST_NAME}
  )
