# parallel-shader-test

This sample application tests KHR_parallel_shader_compile

## Getting Started

You must have a driver with OpenGL 4.6 support.

Building this requires CMake, GLFW, and GLEW.

GLFW - sudo apt-get install libglfw3
GLEW - http://glew.sourceforge.net/install.html (Default install is fine)

To compile, from the project root, run

```
mkdir build
cd build
cmake ..
make
```

The binaries will be put in a bin directory in the project root.
To execute on linux, first source setup-env.sh to add libGLEW to your path. 

```
source setup-env.sh
```

Then just run ./main in bin.

As noted in source, you will need to modify the .glsl files to force a shader recompile. A comment is sufficient for this.

## Sample Output

On a Nvidia 1060 w/ Linux Mint 19.1 Cinnamon I get:

```
OpenGL version: 4.6.0 NVIDIA 390.77
KHR_parallel_shader_compile enabled!
Error: Shader compilation finished immediately!
Error: Shader compilation finished immediately!
Shader is compiled!
Shader is compiled!
Error: Shader compilation finished immediately!
Error: Shader compilation finished immediately!
Shader is compiled!
Shader is compiled!
Program still linking!
Program is linked!
Program is linked!
```
