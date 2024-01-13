# Admirals

Game engine focused on 2D (grid-based), real-time strategy (RTS), multiplayer games. 

[![GitHub Actions Status](https://github.com/joelsiks/admirals/actions/workflows/main.yml/badge.svg)](https://github.com/joelsiks/admirals/actions/workflows/main.yml)


# Installation and Building

## Prerequisites
`Vulkan`, `SDL2`, `CMake`, build system (like `make` or `ninja`) and a C++ and C (with support for C99) compiler.
 
Additional tools/libraries that are used by this project are:
`clang-format` (code formatting), `clang-tidy` (linting), `valgrind` with callgrind (profiling) and `CPPUnit`.

## Cloning

Start by cloning this repository and downloading all the submodules using:
```bash
git clone --recurse-submodules https://github.com/joelsiks/admirals.git
```

You are also able to download/update submodules using:
```bash
git submodule update --init --recursive
```

Due to bad practice (and platform in-portability), the author(s) of Vulkan2D have named the include paths for SDL2 in Linux fashion, resulting in Windows developers not being able to include SDL using the *correct* paths. To fix this, run the following command which replaces the bad include paths with the correct format.
```bash
find include/Vulkan2D/VK2D -type f -exec sed -i 's/<SDL2\//</g' {} +
```

## Building on Windows (using MSYS2)

Download and install the latest Vulkan SDK [here](https://vulkan.lunarg.com/) and make sure that the `VULKAN_SDK` environment variable points to the correct installation path.

Download the latest development package of SDL2, named `SDL2-devel-<version>-VC.zip`, from [here](https://github.com/libsdl-org/SDL/releases) and extract it to the `include/` directory of the project and rename it to `SDL2`.

Download Git Bash or MSYS2 and execute the following commands to install needed build tools (we suggest using the Universal C Runtime (ucrt)):
```bash
pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain mingw-w64-ucrt-x86_64-ninja mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-cppunit
```

You can then move on to creating build files:
```bash
cmake -S . -B build -G Ninja -DCMAKE_C_COMPILER:FILEPATH=C:/msys64/ucrt64/bin/gcc.exe -DCMAKE_CXX_COMPILER=C:/msys64/ucrt64/bin/g++.exe
```

## Building on Linux

Although the game engine mainly supports Windows and likely works best there, the engine can be built on Linux due to the portability of the project's dependencies.

You will need to meet the prerequisites listed [here](#prerequisites). Otherwise, the instructions are very similar to those listed for [Windows](#building-on-windows-using-msys2).

# Styling

## Formatting

This project uses clang-format to format source code, with a config file called `.clang-format` in the project root. clang-format is part of the clang suite, which can be downloaded using the following commands:

```bash
# MSYS2
pacman -S --needed mingw-w64-ucrt-x86_64-clang

# Ubuntu
sudo apt-get install clang-format
```

Run the following command to format a single source file:
```bash
clang-format --style=file -i <source_file>
```

To format all files in the `src/` directory, run:
```bash
find src -name '*.cpp' -o -name '*.hpp' | xargs clang-format --style=file -i
```

Additionally, if using Visual Studio Code, you can download the C/C++ extension, set the default formatter (Ctrl+Shift+P -> Format Document With..) to C/C++ and enable format on save to format while writing code.

## Linting

We use clang-tidy for linting and code consistency. clang-tidy is also part of the clang suite but can be downloaded individually using the following commands:

```bash
# MSYS2
pacman -S --needed mingw-w64-ucrt-x86_64-clang-tools-extra

# Ubuntu
sudo apt-get install clang-tidy
```

Running/using clang-tidy might be different depending on the development environment you are using. There is always the option of running it from the command-line (preferably using `run-clang-tidy`).

For Visual Studio Code users, there is a settings template in `.vscode-template/settings.json` that includes the most important configuration for automatically running clang-tidy on file save, given that you have the [C/C++ extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools) installed. Either move `.vscode-template/settings.json` into `.vsode/settings.json` or copy the contents into your existing `.vscode/settings.json`.

## Unit Tests

We use CppUnit for unit testing. It can be installed using the following commands:
```bash
# MSYS2
pacman -S --needed mingw-w64-ucrt-x86_64-cppunit

# Ubuntu
sudo apt-get install libcppunit-dev
```

# Profiling

The engine (specifically the test programs located in `src/test`) are easiest to profile on Linux using `callgrind` (part of the valgrind tool) to manually interpret using `kcachegrind`. You can install `valgrind` and `kcachegrind` using the following command:
```bash
# Ubuntu
sudo apt-get install valgrind kcachegrind
```

## Instructions

To get results from callgrind that are (reasonably) good, it is preferred to link the Admirals engine statically in the CMakeLists.txt file. This is done by changing the follow line:
```cmake
# From
add_library(engine SHARED ${SOURCES})
# To
add_library(engine STATIC ${SOURCES})
```

Also, to get information about the source code in the profiling results, add the following line to the top of the CMakeLists.txt file. This line adds debug info to the release build type, which is desirable since we also want to profile any optimizations that occur, which isn't the case for the debug build type.
```cmake
set(CMAKE_BUILD_TYPE RelWithDebInfo)
```

### Running profiler
```bash
# Navigate into the build/ directory with build files.
ninja ui_test # or some other executable
valgrind --tool=callgrind ./ui_test
kcachegrind callgrind.out.<pid>
```

# Authors

Joel Sikström, Jakob Wallén, Casper Norrbin, Oskar Gådin
