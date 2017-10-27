# Checkout

```bash
# clone the repo
git clone <repo-url>
# fetch all submodules
git submodule update --init --recursive
```

# Build instructions windows:
Install msys2 http://www.msys2.org/
Use msys2 to install the following:
* install mingw : pacman -S mingw-w64-x86_64-toolchain 
* Install cmake : pacman -S cmake
* Install make : pacman -S make
* Install zlib : pacman -S zlib

From the root of the repository run the following command in the msys MINGW terminal:
    cmake -DCMAKE_TOOLCHAIN_FILE=semantic-adaptation/third_party/into-cpp-fmi/thrird_party/msys-toolchain.cmake .

Now each project can be build using make.

### Eclipse
To import into eclipse use eclipse CDT and select 
file -> import -> Existing Code as Makefile Project
