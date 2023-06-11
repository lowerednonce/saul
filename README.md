# SAUL - SDL Abstraction Ui Library

## Introduction

SAUL is a statically linked c++ library aimed at improving the experience with SDL2. This README will contain installation and general support information, for actual documentation and examples see [the docs folder](docs). The [lib](lib) folder contains the source code for SAUL, while [src](src) contains the source code and assets for a very basic program using every UI element.

## Installation

### Linux

#### Dependencies

On Linux the following packages are needed


| Name            | debian pkg       | arch pkg   |
|-----------------|------------------|------------|
|sdl2             | libsdl2-dev      | sdl2       |
| sdl2-image      | libsdl2-image-dev| sdl2\_image|
|libsdl2-ttf-dev  | libsdl2-ttf-dev  | sdl2\_ttf  |
| cmake           | cmake            | cmake      |
| make            | make             | make       |

#### Building from source

```bash
mkdir build
cd build
cmake ..
make -j$(nproc)
```

### Windows

#### Dependencies

As of v0.2 [SDL2](https://www.libsdl.org/download-2.0.php), [SDL\_Image](https://www.libsdl.org/projects/SDL_image/), [SDL\_TTF](https://www.libsdl.org/projects/SDL_ttf/) are needed to be installed on Windows systems. Along with Visual Studio 2019 minimum. When running place the needed `.dll`s in the PATH or directly in the folder with the executable.

#### Installation

- Go to the windows/ folder.
- Open the solution in Visual Studio 2019 (2019 has been tested, should work on later versions such as 2022).
- Select "release" and "x64" at the top of Visual Studio's Window.
- Right click saul-windows in the Solution Explorer and click "properties".
- Click "C/C++ -> General"
- Click "Additional include directories" and type in the path that leads to your SDL2 header files.
- Right click saul-windows in the Solution Explorer, click "Build".

**WARNING:** In order to load png images libpng16-16.dll is needed

**WARNING:** You might have to edit the #include's at the top of the saul-windows code files depending on the path where your header files are. The default #include's expect the path to have a folder named SDL2, which contains the necessary headers.

**NOTE:** If you want to change your output folder, go to "Configuration Properties -> General" in the properties of the saul-windows project, and type in the path of your output folder in the field called "Output Directory".

## Image rendering support

Linux supports all image rendering with SDL\_image installed. Windows might require additional *.dll* files in the executable's directory or system path. For that see [docs/README.md](docs/README.md)

**NOTE:** The dlls are found inside SDL\_image's runtime binaries zip that can be downloaded over at [https://www.libsdl.org/projects/SDL_image/](https://www.libsdl.org/projects/SDL_image/) under the section *Runtime Binaries* -> *Windows*.
