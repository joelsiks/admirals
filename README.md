# Admirals

Game engine focused on 2D (grid-based), real-time strategy (RTS), multiplayer games. 

# Installation and Building

## Prerequisites
Vulkan, SDL2, CMake and a C++ compiler for Windows (Visual Studio or MSYS2 with g++).

## Cloning

Start by cloning this repository and download all submodules needed:
```bash
git clone --recurse-submodules https://github.com/joelsiks/admirals.git
```

You are also able to download/update depdencies using this command:
```bash
git submodule update --init --recursive
```

Due to bad practice (and platform in-portability), the author(s) of Vulkan2D have named the include paths for SDL2 in Linux fashion, resulting in Windows developers not being able to include SDL using the *correct* paths. To fix this, run the following command which replaces the bad include paths with correct ones.
```bash
find include/Vulkan2D/VK2D -type f -exec sed -i 's/<SDL2\//</g' {} +
```

## Windows (using MSYS2)

Download and install the latest Vulkan SDK [here](https://vulkan.lunarg.com/) and make sure that the `VULKAN_SDK` environment variable points to the correct installation path.

Download the latest development package of SDL2, named `SDL2-devel-<version>-VC.zip`, from [here](https://github.com/libsdl-org/SDL/releases) and extract it to the `include/` directory of the project and rename it to `SDL2`.

Download Git Bash or MSYS2 and execute the following commands to install needed build tools (we suggest using the Universal C Runtime (ucrt)):
```bash
pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain mingw-w64-ucrt-x86_64-ninja mingw-w64-ucrt-x86_64-cmake
```

You can then move on to creating build files:
```bash
$ cmake -S . -B build -G Ninja -DCMAKE_C_COMPILER:FILEPATH=C:/msys64/ucrt64/bin/gcc.exe -DCMAKE_CXX_COMPILER=C:/msys64/ucrt64/bin/g++.exe
```

# Authors

Joel Sikström, Jakob Wallén, Casper Norrbin, Oskar Gådin