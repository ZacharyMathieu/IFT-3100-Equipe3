#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoord;

// colonnes de la matrice
layout(location = 4) in vec4 instanceMatrixCol0;
layout(location = 5) in vec4 instanceMatrixCol1;
layout(location = 6) in vec4 instanceMatrixCol2;
layout(location = 7) in vec4 instanceMatrixCol3;

uniform float uniform_scale;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec2 v_texcoord;
out vec3 v_normal;

void main() {
    mat4 instanceMatrix = mat4(
        instanceMatrixCol0,
        instanceMatrixCol1,
        instanceMatrixCol2,
        instanceMatrixCol3
    );

    vec4 worldPosition = instanceMatrix * vec4(position * uniform_scale, 1.0);
    gl_Position = projectionMatrix * viewMatrix * worldPosition;

    v_texcoord = texcoord;
    v_normal = mat3(transpose(inverse(instanceMatrix))) * normal;
}
