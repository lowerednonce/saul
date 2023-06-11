# SAUL Basic usage

## What is SAUL 

SAUL is a statically linked C++ library that is made to assist the usage of SDL2. It provides classes, structures, a renderer, and other utility functions. SAUL is primarily for 2D screens with 2D objects, as SDL2 itself is not really made for 3D. However, you can implement your own 3D support if you want to.

## Building and running

SAUL uses *CMake* + *GNU Make* + *g++* on GNU/Linux and *Visual Studio* on Microsoft Windows. It compiles to a single static library that can be linked when compiling the project using SAUL. SDL2 is not compiled into the statically linkable archive, thus it is needed to be linked to the final executable. This applies to both officially supported platforms.

### Windows

On Windows, extra *.ddl* files might be needed in the executable's folder for SDL_image to be able to load certain extensions, however those can be found inside the runtime binaries of [SDL_image](https://www.libsdl.org/projects/SDL_image/). The following table demonstrates the needed *.dll* files, :heavy_check_mark: signifies that it *should* work with a base Windows system.

| extension | Windows |
|---|---|
|png|:heavy_check_mark: (libpng16-16.dll required)|
|jpg|:heavy_check_mark: (libjpeg-9.dll required)|
|bmp|:heavy_check_mark: |
|tga|:heavy_check_mark: |
|webp|:heavy_check_mark: (libwebp-7.dll required)|


## SAUL code examples

[Hello World example](HelloWorld.md)
