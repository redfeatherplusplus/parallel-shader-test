# parallel-shader-test

This sample application tests KHR_parallel_shader_compile

## Getting Started

You must have a driver with OpenGL 4.6 support.

Building this requires CMake, GLFW, and GLEW.

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
