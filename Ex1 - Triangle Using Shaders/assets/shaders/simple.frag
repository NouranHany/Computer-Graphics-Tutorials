#version 330

uniform float time;

// Note: need to name this variable as it was named in the out of the vertix shader.
// The Link step in the main.cpp will figure out 
in vec4 vertex_color;

// Can choose any name
// in glsl, out specifies that this is the o/p of the fragment shader
out vec4 frag_color;

// This main will run for each pixel that's covered inside the traingle
// and for each pixel the frag_color, will specify the color for that pixel
void main(){
    vec4 tint = vec4(sin(time), sin(2*time), sin(3*time), 1.0) * 0.5 + 0.5;
    // The fragment shader takes the color calculated from the rasetizer
    // and multiplies it by time, then send it out to the frame buffer.
    frag_color = vertex_color * tint;
    
    // Uncomment this line below to produce a static colors, independent on time
    // frag_color = vertex_color;
}