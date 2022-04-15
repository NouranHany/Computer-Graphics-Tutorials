#version 330

// A uniform variable whose value doesn't change for all runs in 1 draw in the shader
// In other words, in 1 draw, all vertices running this main function in parallel, they'll all see the same uniform value.
// This variable is sent from the main.cpp to the shader
uniform float time;

// Need to send the color as output
// Note we didn't need to do so for the positions, since we passed them to the reasterizer using the built in gl_Position
out vec4 vertex_color;

// This language is glsl (similar somehow to c++)
void main(){
    // assume the data of vertices hardcoded in the shader
    // next Exercise we'll send the vertices data from the main
    // inputed to the shader

    // an array of three vertices
    // each vertix is a 3D vector

    // The Z-coordinate represent the depth
    // We're drawing in 2D, so we specified Z with 0.0 
    // Don't add a colon after the last element
    const vec3 positions[3] = vec3[3](
        vec3(-0.5, -0.5, 0.0),
        vec3( 0.5, -0.5, 0.0),
        vec3( 0.0,  0.5, 0.0)
    );

    // This array, consist of 3 colors, each color will be assigned to a vertix
    // This data will then be sent to the rasterizer.
    // It will do interpolation to calculate a color corresponding to each pixel in the triangle
    // Then send this color calculated to the fragment shader.
    const vec4 colors[3] = vec4[3](
        vec4(1, 0, 0, 1),
        vec4(0, 1, 0, 1),
        vec4(0, 0, 1, 1)
    );

    // Each time this main runs, the gl_VertexID will be a different number
    // From the c++ we'll specify how many vertices,
    // and based on it, the gl_VertexID will take a value
    // The values gl_VertexID will take, will be known at compilation time.
    
    // gl_Position is built-in
    // gl_Position is a 4D vector , It's a homogenous coordinates
    // This line scales the triangles vertices by the sin(time)
    gl_Position = vec4(positions[gl_VertexID] * sin(time), 1.0);
    vertex_color = colors[gl_VertexID];
}