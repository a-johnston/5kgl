# 5KGL
More or less the suite of features I would want on hand if I were to ever develop a game in OpenGL. Built to be small and portable and hopefully pretty quick.

Originally written for HackCWRU 2016. Beerware 2016.

### Building and running
This project requires the headers and libraries for OpenGL, GLEW, and GLFW.

A demo is included, demo.c, along with a makefile. The makefile currently supports building and installing on linux.

To build and install the library, run from the root directory of the project:
```bash
make
sudo make install
```

Then to build and run the demo,
```bash
make demo
./demo.bin
```

Bam! You should see suzanne.

Currently implemented:
- Shaders
    - Easy mapping of uniforms and vertex attributes
    - Error checking of GLSL with debug output
- Meshes
    - Dynamic modification possible
    - Easy VBO creation and use
    - Easily expanded set of attributes
- Math things
    - 2D, 3D vectors
    - Quaternions
    - 4x4 Matrices
    - Helper functions for cameras
-GL Context and Input handling
    - Built around GLFW
    - Super portable

Hopefully coming in the future:
- Sweet loading utilities
    - Textures, meshes, etc
- Simple physics
    - Not looking to remake bullet or anything actually useful
    - Just simple hull collisions and raycasting likely
- Someone who actually knows C
    - Serious code review necessary
