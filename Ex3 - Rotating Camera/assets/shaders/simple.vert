#version 330

uniform mat4 MVP;

layout(location=0) in vec3 position;
layout(location=1) in vec4 color;

out vec4 vertex_color;

void main(){
    // MVP * vertix_coordinates: Transform the vertix from local space to the homogenous clip space
    gl_Position = MVP * vec4(position, 1.0);
    vertex_color = color;
}