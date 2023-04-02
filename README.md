# CodeMap

**CodeMap** is a simple graphical editor for creating maps from screenshots. It allows you to create visual maps and diagrams of source code using arrows. The application provides a user-friendly and minimal interface.

## Features

* Visualize source code using screenshots, arrows and highlights.
* Export project to SVG format (coming soon 🚧)
* Open project from SVG format (coming soon 🚧)

## Building

This project uses [Premake5](https://premake.github.io/download) to generate project files. 

### Windows
Run Premake in the root folder of the project:

```
premake5 vs2022
```

Then use Visual Studio IDE to compile the CodeMap binary.

### Linux
TBD 🚧

## Dependencies

The project relies on a various open source C++ libraries. Most of them are included using git submodules so make sure to clone the repository with `--recurse-submodules` option. All libraries are located at `/lib` directory.

* **clip** - multiplatform cliboard implementation
* **entt** - fast and reliable entity-compontn system
* **glm** - mathemathics library
* **imgui** - GUI library
* **raylib** - simple graphics API that uses OpenGL underneath
* **spdlog** - logging library
* **utfcpp** - library for handling Unicode encoded text

Note: raylib library was modified for the purpose of this project. Some symbols had to be modified to prevent conflicts with `<Windows.h>`.

## License

Copyright © 2023 Sebastian Fojcik \
Use of this software is granted under the terms of the MIT License.

See the [LICENCE](LICENSE) for the full license text.