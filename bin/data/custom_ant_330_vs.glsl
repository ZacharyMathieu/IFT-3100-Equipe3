#version 330 core

in vec3 position;
in vec2 texcoord;

out vec2 vTexcoord;

uniform mat4 modelViewProjectionMatrix;


void main() {
    vTexcoord = texcoord;
    gl_Position = modelViewProjectionMatrix * vec4(position, 1.0);
}
