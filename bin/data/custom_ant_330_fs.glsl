#version 330 core

in vec2 vTexCoord;
in vec3 vFragPos;
in vec3 vNormal;

uniform sampler2D baseColorMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 upperTint; // Teinte à appliquer aux zones grises
uniform vec3 bottomTint;

out vec4 fragColor;

void main() {
    // vec3 albedo = texture(baseColorMap, vTexCoord).rgb;

    // // Détection approximative du gris
    // float grayThreshold = 0.08;
    // bool isGray = abs(albedo.r - albedo.g) < grayThreshold &&
    //               abs(albedo.r - albedo.b) < grayThreshold &&
    //               abs(albedo.g - albedo.b) < grayThreshold;

    // if (isGray) {
    //     albedo *= upperTint;
    // }
    // else{
    //     albedo *= bottomTint;
    // }

    // float metallic = texture(metallicMap, vTexCoord).r;
    // float roughness = texture(roughnessMap, vTexCoord).r;

    // vec3 N = normalize(vNormal);
    // vec3 L = normalize(lightPos - vFragPos);
    // vec3 V = normalize(viewPos - vFragPos);
    // vec3 H = normalize(L + V);

    // float diff = max(dot(N, L), 0.0);
    // float spec = pow(max(dot(N, H), 0.0), 32.0);
    // vec3 specColor = mix(vec3(0.04), albedo, metallic);
    // vec3 color = albedo * diff + specColor * spec;

    // fragColor = vec4(color, 1.0);
    fragColor = vec4(1,0,0,1);
}
