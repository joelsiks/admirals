# Admirals

Game engine focused on 2D (grid-based), real-time strategy (RTS), multiplayer games. 

# Installation and Building
## Prerequisites
Prerequisites: `Vulkan`, `SDL2`, `CMake`

### Installation Option 1
Download and install the latest Vulkan SDK [here](https://vulkan.lunarg.com/) and make sure that the `VULKAN_SDK` environment variable points to the corrent installation path.

For Windows you have to download the development package `SDL2-devel-<version>-VC.zip` from [here](https://github.com/libsdl-org/SDL/releases) and extract it to the root directory of the project and rename it to `SDL2`.

### Installation Option 2
Alternatively, the dependencies can be downloaded using MSYS2 with `packman`:
`pacman -S mingw-w64-x86_64-vulkan-headers`
`pacman -S mingw-w64-x86_64-vulkan-loader`
`pacman -S mingw-w64-x86_64-SDL2`

## Submodules
Pull down the latest version of the submodules using:
`git submodule update --init --recursive`

Due to bad practice (and platform in-portability), the author(s) of Vulkan2D have named the include paths for SDL2 in Linux fashion, resulting in Windows developers not being able to include SDL using the *correct* paths. To fix this, run the following command which replaces the bad include paths with correct ones.
```bash
find Vulkan2D/VK2D -type f -exec sed -i 's/<SDL2\//</g' {} +
```

## Building
Then you can move on to create the buildfiles using:
```bash
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
```
or
```bash
cmake -S . -B build -G "Unix Makefiles" -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE "-DCMAKE_CXX_COMPILER:FILEPATH=.../g++.exe"
```

# Authors

Joel Sikström, Jakob Wallén, Casper Norrbin, Oskar Gådin