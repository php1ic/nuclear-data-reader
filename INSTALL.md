# Nuclear Data Reader Installation


## [CMake](https://cmake.org/)

In-source builds are blocked so compilation needs to be done in a separate build directory.
I prefer entirely separate from the source so that is what these commands do.
Assuming we are currently in the root of the project, the following will build the executable.

```bash
cd ../ && mkdir build && cd build
cmake ../nuclear-data-reader
make
```
Or, to be build tool agnostic, again from the source root
```bash
cmake -H. -B../build
cmake --build ../build
```

This will create a *bin/* directory (inside the build directory) and place the executable inside.

The option `CMAKE_INSTALL_PREFIX` can be passed to cmake to specify the install location if required.

As part of the build, the binary is coded to read files located in */your/build/path/nuclear-data-reader/data/*.
Thus the executable can be copied into one of your $PATH directories and it will still be able to find the necessary files.
