#version 330 core

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;

in vec3 position;
in vec3 normal;
in vec2 texcoord;

out vec2 vTexCoord;
out vec3 vFragPos;
out vec3 vNormal;

void main() {
    vTexCoord = texcoord;
    vec4 worldPos = modelMatrix * vec4(position, 1.0);
    vFragPos = worldPos.xyz;
    vNormal = normalize(normalMatrix * normal);

    gl_Position = modelViewProjectionMatrix * vec4(position, 1.0);
}
