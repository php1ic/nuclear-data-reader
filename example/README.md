# Example usage

Small example demonstrating how the library could be used in another project.
For this example, it is assumed that you have installed into `build/install` within the root of the project.

Configure and build with
```bash
cmake -S. -Bbuild
cmake --build ./build
```

Once built, the executable will read the data files from each year and write it out as a json file.
