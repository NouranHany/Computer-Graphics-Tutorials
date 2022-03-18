#version 330

uniform float time;

in vec4 vertex_color;
out vec4 frag_color;

void main(){
    vec4 tint = vec4(sin(time), sin(2*time), sin(3*time), 1.0) * 0.5 + 0.5;
    frag_color = vertex_color;// * tint;
}