# opengl-sandbox

WIP.

experiment environment for realtime graphics methods in OpenGL.

## Requirements

* C++17
* CMake 3.12 or Higher
* OpenGL 3.3

## Features

* Blinn-Phong Shading
* Normal Mapping
* Shadow Mapping

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

Hello world in OpenGL.

![](./img/triangle.jpg)

### model-viewer

Visualize 3d models with various layers such as Position, Normal, TexCoords, Diffuse, ...

![](./img/model-viewer.jpg)

### tangent-space

Visualize basis of tangent space at each vertex using geometry shader.

![](./img/tangent-space.jpg)

### simple-shading

Blinn-Phong shading example with moving point light.

![](./img/simple-shading.jpg)

### normal-mapping

Normal mapping example with Blinn-Phong shading.

![](./img/normal-mapping-compare.jpg)

### bump-mapping

Bump mapping example using height map with Blinn-Phong shading.

![](./img/bump-mapping-compare.jpg)

### shadow-mapping

Shadow mapping example with moving directional light.

![](./img/shadow-mapping3.jpg)

### omnidirectional-shadow-mapping

Omnidirectional shadow mapping example with moving point light.

![](./img/omnidirectional-shadow-mapping.jpg)

## Externals

* [glad](https://github.com/Dav1dde/glad)
* [glm](https://github.com/g-truc/glm)
* [GLFW](https://github.com/glfw/glfw)
* [assimp](https://github.com/assimp/assimp)
* [imgui](https://github.com/ocornut/imgui)
* [stb](https://github.com/nothings/stb)
* [GLSL Shader Includes](https://github.com/tntmeijs/GLSL-Shader-Includes)

## References

* [Learn OpenGL](https://learnopengl.com/)
* [Project ASURA](http://www.project-asura.com/)