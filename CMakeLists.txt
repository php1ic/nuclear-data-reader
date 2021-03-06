# Require (i.e. demand) an out-of-source build
if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
  message("")
  message(STATUS "Please use an out-of-source build, it's neater.")
  message(STATUS "Don't forget to clean up ${CMAKE_BINARY_DIR} by removing:")
  message(STATUS "\tCMakeCache.txt")
  message(STATUS "\tCMakeFiles")
  message(STATUS "Then you can create a separate directory and re-run cmake from there.\n")
  message(FATAL_ERROR "In-source build attempt detected")
endif()

# code coverage as a link option arrived in 3.13
cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
message(STATUS "CMake version: ${CMAKE_VERSION}")

# Put all binaries in a their standard locations
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Project name, along with language type
project(
  nuclear-data-reader
  HOMEPAGE_URL "https://github.com/php1ic/nuclear-data-reader"
  LANGUAGES CXX
  VERSION 0.0.1
  )

# Add the 'cmake' folder to the list of paths to check for modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# Set some global project settings
include(GeneralProjectSettings)

# Configure static analysers, this was done via scripts, but cmake can now do it
include(StaticAnalyzers)

# Link this 'library' to set the c++ standard / compile-time options requested
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE)

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)

# Which compiler flags should we use
include(CompilerWarnings)
set_project_warnings(project_warnings)

# Doxygen configuration
include(Doxygen)
enable_doxygen()

# Code Coverage Configuration
include(CodeCoverage)
enable_codecoverage(project_options)

# Allow the various santizers to be toggled ON/OFF
include(Sanitizers)
enable_sanitizers(project_options)

# Make sure any and all external libraries are linked
include(ExternalLibraries)
add_external_libraries(project_options)

# Configurable header file to store project version number
add_subdirectory(include/${PROJECT_NAME})

# Keep list alphabetical
set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(SOURCES
  ${SOURCE_DIR}/ame_data.cpp
  ${SOURCE_DIR}/converter.cpp
  ${SOURCE_DIR}/massTable.cpp
  ${SOURCE_DIR}/nubase_data.cpp
  ${SOURCE_DIR}/isotope.cpp
  )

add_library(${PROJECT_NAME} SHARED ${SOURCES})

# Where are the header files
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include/)

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE
  project_warnings
  project_options
  )

# Unit testing with Catch2
option(NDR_UNIT_TESTS "Build unit tests" OFF)
if(NDR_UNIT_TESTS)
  enable_testing()
  add_subdirectory(external/Catch2)
  add_subdirectory(tests)
endif()

# Setup an install target
install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)
install(DIRECTORY include/${PROJECT_NAME} DESTINATION include FILES_MATCHING PATTERN "*.hpp")
