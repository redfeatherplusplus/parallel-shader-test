# parallel-shader-test

This sample application tests KHR_parallel_shader_compile

## Getting Started

You must have a driver with OpenGL 4.6 support.

Building this requires CMake, GLFW, and GLEW.

* GLFW - sudo apt-get install libglfw3
* GLEW - http://glew.sourceforge.net/install.html (Default install is fine)

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

As noted in source, you will need to modify the .glsl files to force a shader recompile. 
A comment is sufficient for this. 

After running, it is suggested to try the 'big' shaders.
This shows the application halt with GL_COMPLETION_STATUS_KHR for shaders. 
Note: halting occurs with the trivial shaders as well, as indicated by:
"Error: Shader compilation finished immediately!" The 'big' shaders just
make complilation take long enough for the delay to be humanly-percieved.

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

With 'big' shaders my app halts for 3~5 minutes for each big shader on a i7 7700k.
I haven't gotten it to complete linking with 'big' shaders yet (Waited 25 mins).
