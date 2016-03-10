## 5KGL
More or less the suite of features I would want on hand if I were to ever develop a game in OpenGl. Built to be small and portable and hopefully pretty quick.

Originally written for HackCWRU 2016.

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
- Sweet textures
    - Mostly hoping for loading utilities
- Scenes
    - Sort of up in the air since draw function is left to user
    - Pretty open ended at the moment
- Rendering hints
    - At the moment, way to eager to pass data that might not need to be passed every time
- Someone who actually knows C
    - Serious code review necessary
