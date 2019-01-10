Add a new Semantic Adaptation:
1. Duplicate the project from an existing sample.
1. Edit the `.project`, `CMakeLists.txt`, `create_fmu.ps1`, files
1. Edit the `main.cpp`, `modelDescription.xml`, files in src-gen
1. Edit the `hybrid-cosim-framework/CMakeLists.txt` file to include the new project.
1. Open MSYS 64-bits and cd to `hybrid-cosim-framework`.
1. Run
```
cmake -DCMAKE_TOOLCHAIN_FILE=semantic-adaptation/third_party/into-cpp-fmi/thrird_party/msys-toolchain.cmake .
```
1. Run make:
```
make
```
1. Run the main app:
```
make
```
