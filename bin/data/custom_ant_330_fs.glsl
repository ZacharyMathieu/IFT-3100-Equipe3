#version 330 core

in vec2 vTexcoord;
out vec4 fragColor;

uniform sampler2D baseColorMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;

void main() {
    vec3 baseColor = texture(baseColorMap, vTexcoord).rgb;
    vec3 normal = texture(normalMap, vTexcoord).rgb;
    float metallic = texture(metallicMap, vTexcoord).r;
    float roughness = texture(roughnessMap, vTexcoord).r;

    // Tu peux afficher ce que tu veux, par exemple :
    // fragColor = vec4(baseColor, 1.0); // juste la couleur de base
    // fragColor = vec4(normal, 1.0);    // ou la normal map
    // fragColor = vec4(vec3(metallic), 1.0); // ou la metallic
    // fragColor = vec4(vec3(roughness), 1.0); // ou la roughness

    // Exemple : on combine tout visuellement (juste pour affichage)
    fragColor = vec4(baseColor.r, metallic, roughness, 1.0);
}
