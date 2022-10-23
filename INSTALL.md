# Nuclear Data Reader Installation


## [CMake](https://cmake.org/)

In-source builds are blocked so compilation needs to be done in a separate build directory.
Assuming we are currently in the root of the project, the following will build the library.

```bash
mkdir build && cd build
cmake ../
make
```
Or, to be build tool agnostic, again from the source root
```bash
cmake -H. -B./build
cmake --build ./build
```

The option `CMAKE_INSTALL_PREFIX` can be passed to cmake to specify the install location as required.

As part of the build, the library is coded to read files located in */your/build/path/nuclear-data-reader/data/*.

# Presets

CMake introduced [preset](https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html) in 3.19 to allow simpler, and shared, configs.
If you have a new enough version of CMake, you can do all or one of the following to configure, build and test.
```bash
# Configure
cmake --preset <configure-preset>
# Build
cmake --build --preset <build-preset>
# Test
ctest --preset <test-preset>
```

All presets are defined and set up in [CMakePresets.json](./CMakePresets.json) and can be listed with
```bash
cmake --list-presets
```
