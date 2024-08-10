# KrisRaycaster
Ray caster 3D renderer (Wolfenstein 3D style) for STM32 embedded platform. The aim of the project is to play around with 1990's rendering techniques in an environment that has very limited resources.

# Build
## STM32
WIP

## Desktop
This project uses `vcpkg` toolchain to manage dependencies (SDL2) and `CMake` as a build system.

If you have `vcpkg` installed already, you can use the global path as your `VCPKG_ROOT` in `CMakeUserPresets.json`. Otherwise, the git submodule will be used.

Make sure to clone the repository with: `git clone --recursive`. If you have already cloned the project, run `git submodule update --recursive --remote`.

The project uses `ninja` generator. It should automatically be found when using IDEs such as Visual Studio and CLion. When using VSCode on Windows, you might have to install it (`choco install ninja`) and make sure it's in the PATH.
