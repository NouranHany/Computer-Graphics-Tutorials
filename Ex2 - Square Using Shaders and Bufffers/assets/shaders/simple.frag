#version 330

uniform float time;

// Difference between uniform, attribute variable, and varying variable.
// ----------
// uniform are per-primitive parameters (constant during an entire draw call) ;

// Variables that are 'in' into the vshader are called 'attributes'
// attribute are per-vertex parameters (typically : positions, normals, colors, UVs, ...) ;

// Variables that are 'out' in vshader, and 'in' into fshader are called 'varying' (e.g vertex_color)
// varying are per-fragment (or per-pixel) parameters : they vary from pixels to pixels.

// e.g vertex_color is a varying variable for each vertex of a triangle inside the vertex shader. 
// When this varying parameter is sent to the fragment shader, 
// its value is automatically interpolated based on the position of the pixel to draw.
// Thus vertex_color in the fshader is the interpolated color for the current running pixel.
in vec4 vertex_color;
out vec4 frag_color;

void main(){
    vec4 tint = vec4(sin(time), sin(2*time), sin(3*time), 1.0) * 0.5 + 0.5;
    frag_color = vertex_color;// * tint;
}