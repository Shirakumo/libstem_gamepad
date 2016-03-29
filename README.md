## About
This is a small, portable library to handle USB gamepad input devices.

## Using
See `source/gamepad/Gamepad.h` for information about the API or [cl-gamepad](https://github.com/Shirakumo/cl-gamepad) for Common Lisp bindings for it.

## Building
Building shouldn't be much of a problem.

### Unix
Simply run `make` from the root of the repository. The produced shared library will be in the `build` directory. You will need GCC or Clang, GNUMake and cmake.

### Windows
Launch the Visual Studio developer environment console and run:

    mkdir build
    cd build
    cmake -G "NMake Makefiles" ..
    nmake

The produced DLL will be in the `build` directory. Aside from Visual Studio and cmake you will need the WindowsSDK.
