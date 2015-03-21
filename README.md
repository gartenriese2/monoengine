# monoEngine
single threaded opengl engine

## Building ##

If you want to use GCC instead of Clang, use `make COMPILER=g++`. Otherwise, Clang++ will be used.

## Dependencies ##

### Linux ###

* `cmake`
* Either `g++ 4.9` or `clang++ 3.5`
* `libx11-dev`, `libxrandr-dev`, `libxi-dev`, `libxxf86vm-dev` for OpenGL
* Graphics Drivers that support OpenGL 4.5 (OpenGL 3.3 in legacy mode)

### Windows (experimental) ###

* `cmake`
* Visual Studio 2015 RC
* Graphics Drivers that support OpenGL 4.5 (OpenGL 3.3 in legacy mode)
