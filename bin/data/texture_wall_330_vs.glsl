#version 330

in vec4 position;
in vec4 normal;
in vec2 texcoord;
in vec3 tangent; // ← nécessaire pour normal map

out vec3 surface_position;
out vec3 surface_normal;
out vec2 surface_texcoord;
out mat3 tbn_matrix; // ← nécessaire pour normal map

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform vec3 translation;
uniform float scale_factor;

void main()
{
    // Appliquer translation + échelle
    vec4 transformed_position = vec4(position.xyz * scale_factor + translation, 1.0);

    // Calcul normal
    mat4 normalMatrix = transpose(inverse(modelViewMatrix));
    vec3 N = normalize(vec3(normalMatrix * normal));
    vec3 T = normalize(vec3(modelViewMatrix * vec4(tangent, 0.0)));
    vec3 B = cross(N, T);

    tbn_matrix = mat3(T, B, N);

    surface_normal = N;
    surface_position = vec3(modelViewMatrix * transformed_position);
    surface_texcoord = texcoord;

    gl_Position = projectionMatrix * modelViewMatrix * transformed_position;
}
