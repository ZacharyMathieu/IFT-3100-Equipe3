#version 330

in vec4 position;
in vec4 normal;
in vec2 texcoord;
in vec3 tangent;

out vec3 surface_position;
out vec3 surface_normal;
out vec2 surface_texcoord;
out mat3 tbn_matrix;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform vec3 translation;
uniform float scale_factor;

void main()
{
    vec4 transformed_position = vec4(position.xyz * scale_factor + translation, 1.0);

    mat3 normalMatrix = transpose(inverse(mat3(modelViewMatrix)));
    vec3 N = normalize(normalMatrix * normal.xyz);
    vec3 T = normalize(mat3(modelViewMatrix) * tangent);
    vec3 B = cross(N, T);

    tbn_matrix = mat3(T, B, N);

    surface_normal = N;
    surface_position = vec3(modelViewMatrix * vec4(position.xyz * scale_factor + translation, 1.0));

    surface_texcoord = texcoord;

    gl_Position = projectionMatrix * modelViewMatrix * transformed_position;
}
