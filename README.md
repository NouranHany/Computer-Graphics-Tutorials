# Computer-Graphics-Tutorials
<p align="center">
  <a style="text-decoration:none" >
    <img src="https://img.shields.io/badge/Language-C%2B%2B-blue" alt="Website" />
  </a>
  <a style="text-decoration:none" >
    <img src="https://img.shields.io/badge/Shader%20Language-GLSL-blue" alt="Website" />
  </a>
  <a style="text-decoration:none" >
    <img src="https://img.shields.io/badge/Graphics%20API-OpenGL-blue" alt="Website" />
  </a>
  <a style="text-decoration:none" >
    <img src="https://img.shields.io/badge/Build%20Generator%20Tool-CMake-blue" alt="Website" />
  </a>
</p>


This repo contains the CMP 3060 tutorials. It include tutorials for the Computer graphics and Human Computer Inmteraction course. Including well explanatory comments for  the code.

# Outlines
## Triangle using Shaders
- Drawing a triangle using vertex and fragment shaders.
- Uniforms are used to send time to make the traingle shrink and expand by time.
- For this exercise data of the triangles is hard-coded inside the shaders.

## Square using Shaders and Buffers
- In this exercise, Array Buffers are used to send data from main.cpp to the shaders instead of hard-coding them into shaders.
- Also Elements Buffer is introduced to let you draw a square using 4 vertices instead of 6.
- Location of attributes are explicitly specified to let pass same data to multiple shaders having attributes in same locations.
- 
