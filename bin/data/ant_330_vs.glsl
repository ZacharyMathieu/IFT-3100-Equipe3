#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoord;

layout(location = 4) in vec4 instanceMatrixCol0;
layout(location = 5) in vec4 instanceMatrixCol1;
layout(location = 6) in vec4 instanceMatrixCol2;
layout(location = 7) in vec4 instanceMatrixCol3;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform float uniform_scale;

out vec3 vNormal;
out vec2 vTexCoord;
out vec3 vWorldPos;

void main() {
    mat4 model = mat4(instanceMatrixCol0, instanceMatrixCol1, instanceMatrixCol2, instanceMatrixCol3);
    vec4 worldPos = model * vec4(position * uniform_scale, 1.0);

    vWorldPos = worldPos.xyz;
    vNormal = mat3(transpose(inverse(model))) * normal;
    vTexCoord = texcoord;

    gl_Position = projectionMatrix * viewMatrix * worldPos;
}
