#version 330 core

const float PI = 3.14159265359;

in vec3 surface_position;
in vec3 surface_normal;
in vec2 surface_texcoord;

out vec4 fragment_color;

uniform vec3 material_color_ambient;
uniform vec3 material_color_diffuse;
uniform vec3 material_color_specular;

uniform float material_brightness;
uniform float material_metallic;
uniform float material_roughness;
uniform float material_occlusion;
uniform vec3 material_fresnel_ior;

uniform float tone_mapping_exposure;
uniform bool tone_mapping_toggle;
uniform float tone_mapping_gamma;

uniform vec3 light_position;
uniform vec3 light_color;
uniform float light_intensity;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_metallic;
uniform sampler2D texture_roughness;
uniform sampler2D texture_occlusion;

uniform sampler2D baseColorMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;

uniform bool isAnt;
uniform vec3 upperTint;
uniform vec3 bottomTint;

uniform bool isMirror;

float trowbridge_reitz(vec3 n, vec3 h, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float ndh = max(dot(n, h), 0.0);
    float ndh2 = ndh * ndh;
    float denom = (ndh2 * (a2 - 1.0) + 1.0);
    return (a2 / (PI * denom * denom));
}

float schlick_beckmann(float costheta, float roughness) {
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;
    return costheta / (costheta * (1.0 - k) + k);
}

float smith(vec3 n, vec3 l, vec3 v, float roughness) {
    float ndl = max(dot(n, l), 0.0);
    float ndv = max(dot(n, v), 0.0);
    return schlick_beckmann(ndl, roughness) * schlick_beckmann(ndv, roughness);
}

vec3 schlick_fresnel(float costheta, vec3 f0) {
    return f0 + (1.0 - f0) * pow(1.0 - costheta, 5.0);
}

vec3 tone_mapping(vec3 color) {
    color = vec3(1.0) - exp(-color * tone_mapping_exposure);
    return tone_mapping_toggle ? 
        clamp((color * (2.51 * color + 0.03)) / (color * (2.43 * color + 0.59) + 0.14), 0.0, 1.0)
        : color / (color + vec3(1.0));
}

vec3 evaluateBRDF() {
    vec3 n = normalize(surface_normal);
    vec3 v = normalize(-surface_position);
    vec3 l = normalize(light_position - surface_position);
    vec3 h = normalize(v + l);

    vec3 albedo;

    if (isAnt) {
        vec3 texColor = texture(baseColorMap, surface_texcoord).rgb;
        float grayThreshold = 0.08;
        bool isGray = abs(texColor.r - texColor.g) < grayThreshold &&
                      abs(texColor.r - texColor.b) < grayThreshold &&
                      abs(texColor.g - texColor.b) < grayThreshold;

        albedo = texColor * (isGray ? upperTint : bottomTint);
    } else {
        vec3 texColor = texture(texture_diffuse, surface_texcoord).rgb;
        albedo = material_color_diffuse * pow(texColor, vec3(tone_mapping_gamma));
    }

    float metallic = isAnt ? texture(metallicMap, surface_texcoord).r : material_metallic * texture(texture_metallic, surface_texcoord).r;
    float roughness = isAnt ? texture(roughnessMap, surface_texcoord).r : material_roughness * texture(texture_roughness, surface_texcoord).r;
    float occlusion = isAnt ? 1.0 : material_occlusion * texture(texture_occlusion, surface_texcoord).r;

    vec3 ambient = material_color_ambient * albedo * occlusion;

    float d = trowbridge_reitz(n, h, roughness);
    float g = smith(n, l, v, roughness);
    vec3 f0 = mix(material_fresnel_ior, albedo, metallic);
    vec3 f = schlick_fresnel(max(dot(h, v), 0.0), f0);
    vec3 spec = (d * f * g) / max(4.0 * max(dot(n, v), 0.0) * max(dot(n, l), 0.0), 0.001);

    spec *= material_color_specular;

    vec3 kd = (vec3(1.0) - f) * (1.0 - metallic);
    vec3 diffuse = kd * albedo / PI;

    float dist = length(light_position - surface_position);
    float attenuation = 1.0 / (0.01 + 0.000001 * dist + 0.0000001 * dist * dist);
    vec3 radiance = light_color * attenuation * light_intensity;

    vec3 color = (ambient + (diffuse + spec) * radiance * max(dot(n, l), 0.0)) * material_brightness;
    return color;
}

void main() {
    vec3 color = evaluateBRDF();
    
    if (isAnt && isMirror) {
        // Inversion simple par symétrie sur l'axe Y
        color = vec3(color.r, 1.0 - color.g, color.b);
    }
    color = tone_mapping(color);
    color = pow(color, vec3(1.0 / tone_mapping_gamma));
    fragment_color = vec4(color, 1.0);
}
