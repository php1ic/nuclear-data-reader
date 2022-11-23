cmake_minimum_required(VERSION 3.13)
project(ndr_test)

set(EXE ndr)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

set(NDR_PATH "${CMAKE_SOURCE_DIR}/../build/install")

include_directories("${NDR_PATH}/include")
link_directories("${NDR_PATH}/lib")

add_executable(${EXE} ndr.cpp)
target_link_libraries(${EXE} nuclear-data-reader)
