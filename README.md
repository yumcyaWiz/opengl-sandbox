# opengl-sandbox

WIP.

experiment environment for realtime graphics methods in OpenGL.

## Requirements

* C++17
* CMake 3.12 or Higher
* OpenGL 3.3

## Features

* Blinn-Phong Shading

## Setup

```bash
git submodule update --init
```

## Build

```bash
mkdir build
cd build
cmake ..
make
```

## Gallery

### triangle

hello world in OpenGL.

![](./img/triangle.jpg)

### simple-shading

blinn-phong shading example with moving point light.

![](./img/simple-shading.jpg)

### shadow-mapping

shadow mapping example with moving directional light.

![](./img/shadow-mapping3.jpg)

## Externals

* [glad](https://github.com/Dav1dde/glad)
* [glm](https://github.com/g-truc/glm)
* [GLFW](https://github.com/glfw/glfw)
* [assimp](https://github.com/assimp/assimp)
* [imgui](https://github.com/ocornut/imgui)
* [stb](https://github.com/nothings/stb)
* [GLSL Shader Includes](https://github.com/tntmeijs/GLSL-Shader-Includes)