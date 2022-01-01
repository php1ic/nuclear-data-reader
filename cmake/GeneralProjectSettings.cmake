# Output compilation database that can be used by analysis tools
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Compile against c++20
set(CMAKE_CXX_STANDARD 20)
# Don't fallback if c++20 is not available
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Use -std=c++?? rather than -std=gnu++??
set(CMAKE_CXX_EXTENSIONS OFF)

# Use Debug as the default build type
if(NOT CMAKE_BUILD_TYPE)
  set(
    CMAKE_BUILD_TYPE "RelWithDebInfo"
    CACHE STRING "Set the build type to use: Debug, Release, RelWithDebInfo (default), MinSizeRel" FORCE
    )
endif()

# Use ccache if it exists on the system and is wanted
option(NDR_CCACHE "Use ccache if available" ON)
if(NDR_CCACHE)
  message(STATUS "[ccache] Looking for executable")
  find_program(CCACHE ccache)

  if(CCACHE)
    message(STATUS "[ccache] Found executable")
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
  else()
    message(WARNING "[ccache] Requested but executable not found")
  endif()
else()
  message(STATUS "[ccache] Not using")
endif()

# Use IPO if possible and wanted
option(NDR_IPO "Use link optimisation/InterProcedural Optimization" ON)
if(NDR_IPO)
  message(STATUS "[IPO/LTO] Checking if supported")
  include(CheckIPOSupported)
  check_ipo_supported(RESULT IPOsupported OUTPUT error)

  if(IPOsupported)
    message(STATUS "[IPO/LTO] Supported so will use")
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
  else()
    message(WARNING "[IPO/LTO] Not supported: <${error}>")
  endif()
else()
  message(STATUS "[IPO/LTO] Will not attempt to use")
endif()
