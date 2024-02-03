# CodeMap

**CodeMap** is a simple graphical editor for creating maps from screenshots. It allows you to create visual maps and diagrams of source code using arrows. The application provides a user-friendly and minimal interface.

![image](https://github.com/KyrietS/CodeMap/assets/19170699/a35fb351-f517-4e55-a35c-3b95cce2696d)


## Features

* Visualize source code using screenshots, arrows and highlights.
* Export project to SVG format
* Open project from SVG format
* Standalone app (the binary weights around 6 MB)
* Supports Windows and Linux

## Building

This project uses CMake to generate project files. It should work the same way on all supported platforms.

```bash
git clone https://github.com/KyrietS/CodeMap.git
cmake -S CodeMap -B CodeMapBuild
cmake --build CodeMapBuild
```

## Dependencies

The project relies on a various open source C++ libraries. All of them are fetched automatically during the CMake configuration process.

* **clip** - multiplatform cliboard implementation
* **entt** - fast and reliable entity-component system
* **glm** - mathemathics library
* **imgui** - GUI library
* **raylib** - simple graphics API that uses OpenGL underneath
* **reasings** - single-header C++ library for easing functions
* **spdlog** - logging library
* **utfcpp** - library for encoding and decoding UTF text
* **tinyxml2** - library for parsing XML files
* **stb** - encoding and decoding images to/from raw data
* **[trex](https://github.com/KyrietS/trex)** - self-made font rasterizer and text shaper library

Note: raylib library was modified for the purpose of this project. Some symbols had to be modified to prevent conflicts with `<Windows.h>`.

## License

Copyright © 2023 Sebastian Fojcik \
Use of this software is granted under the terms of the MIT License.

See the [LICENCE](LICENSE) for the full license text.
