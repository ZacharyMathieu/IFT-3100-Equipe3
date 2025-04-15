#version 330 core

in vec3 vWorldPos;
in vec3 vNormal;
in vec2 vTexCoord;

out vec4 fragColor;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;

uniform vec3 light_position;
uniform vec3 view_position;

const float PI = 3.14159265359;

void main() {
    // Simple fetch
    vec3 albedo = texture(albedoMap, vTexCoord).rgb;
    float metallic = texture(metallicMap, vTexCoord).r;
    float roughness = texture(roughnessMap, vTexCoord).r;
    vec3 normal = texture(normalMap, vTexCoord).rgb * 2.0 - 1.0;

    // World vectors
    vec3 N = normalize(normal);
    vec3 V = normalize(view_position - vWorldPos);
    vec3 L = normalize(light_position - vWorldPos);
    vec3 H = normalize(V + L);

    // Simple fresnel
    vec3 F0 = mix(vec3(0.04), albedo, metallic);
    vec3 F = F0 + (1.0 - F0) * pow(1.0 - dot(H, V), 5.0);

    float NdotL = max(dot(N, L), 0.0);
    vec3 kS = F;
    vec3 kD = (1.0 - kS) * (1.0 - metallic);
    vec3 diffuse = kD * albedo / PI;

    // Very simple specular approx
    vec3 specular = kS;

    vec3 radiance = vec3(1.0);
    vec3 color = (diffuse + specular) * radiance * NdotL;

    color = pow(color, vec3(1.0 / 2.2)); // gamma correct

    fragColor = vec4(color, 1.0);
}
