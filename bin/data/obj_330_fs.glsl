// IFT3100H25 ~ lambert_330_fs.glsl

#version 330

#define light_intensity 0.5

in vec3 frag_position;
in vec3 frag_normal;

out vec4 fragment_color;

uniform vec4 color_ambient;
uniform vec4 light_color;
uniform vec3 light_direction;

void main()
{
    vec3 n = normalize(frag_normal);

    float d = dot(n, light_direction) * light_intensity;

    fragment_color = light_color * d + color_ambient * (1 - d);
}