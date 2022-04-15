#version 330

uniform float time;

// We specify explicity the entry location we wish an attribute to be defined in.
// Thus we don't need to get the locations of these attributes in the main.cpp
// Now, if we want to get the location of a specific attribute in the main.cpp
// It will be the location number we assigned here for that attribute.  

// This way we can specify in many shaders the same location number e.g 3
// And send data from main to this location 3
// then that data will be sent to every attribute @ location 3 in all shaders.
layout(location=0) in vec3 position;
layout(location=1) in vec4 color;

//Note that 1 location takes 4 floats, thus can put up to vec4 in 1 entry location.

out vec4 vertex_color;

void main(){
    gl_Position = vec4(position, 1.0);
    vertex_color = color;
}