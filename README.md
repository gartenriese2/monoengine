# monoEngine
single threaded opengl engine

## Building ##

If you want to use GCC instead of Clang, use `make COMPILER=g++`. Otherwise, Clang++ will be used.

## Dependencies ##

### Linux ###

* `cmake`
* `g++ 5.1` (clang does not yet support auto & braced init)
* `libgl1-mesa-dev`, `libx11-dev`, `libxrandr-dev`, `libxi-dev`, `libxxf86vm-dev` for OpenGL
* Graphics Drivers that support OpenGL 4.5 (OpenGL 3.3 in legacy mode)
  * e.g. use this [PPA](https://launchpad.net/~xorg-edgers/+archive/ubuntu/ppa) for nvidia

### Windows (experimental) ###

* `cmake`
* Visual Studio 2015 RC
* Graphics Drivers that support OpenGL 4.5 (OpenGL 3.3 in legacy mode)
