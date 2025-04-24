#version 330

const float PI = 3.1415926535897932384626433832795;

const float light_attenuation_factor_constant = 0.01;
const float light_attenuation_factor_linear = 0.000001;
const float light_attenuation_factor_quadratic = 0.0000001;

in vec3 surface_position;
in vec3 surface_normal;
in vec2 surface_texcoord;
in mat3 tbn_matrix; // MODIF : ajout pour normal map

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

//  MODIF : textures regroupées
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D armMap;

uniform vec3 light_position;
uniform vec3 light_color;
uniform float light_intensity;

// Fonctions inchangées (trowbridge_reitz, schlick_beckmann, smith, schlick_fresnel, tone_mapping)

float trowbridge_reitz(vec3 n, vec3 h, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float ndh = max(dot(n, h), 0.0);
    float ndh2 = ndh * ndh;
    float numerator = a2;
    float denominator = (ndh2 * (a2 - 1.0) + 1.0);
    denominator = denominator * denominator * PI;
    return numerator / denominator;
}

float schlick_beckmann(float costheta, float roughness) {
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;
    float numerator = costheta;
    float denominator = costheta * (1.0 - k) + k;
    return numerator / denominator;
}

float smith(vec3 n, vec3 l, vec3 v, float roughness) {
    float ndl = max(dot(n, l), 0.0);
    float ndv = max(dot(n, v), 0.0);
    return schlick_beckmann(ndl, roughness) * schlick_beckmann(ndv, roughness);
}

vec3 schlick_fresnel(float costheta, vec3 f0) {
    return f0 + (1.0 - f0) * pow(1.0 - costheta, 5.0);
}

vec3 tone_mapping_reinhard(vec3 x) {
    return x / (x + vec3(1.0));
}

vec3 tone_mapping_aces_filmic(vec3 x) {
    float a = 2.51f;
    float b = 0.03f;
    float c = 2.43f;
    float d = 0.59f;
    float e = 0.14f;
    return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}

vec3 brdf_cook_torrance()
{
    //  MODIF : Normale via normal map
    vec3 normal_map = texture(normalMap, surface_texcoord).rgb;
    vec3 n = normalize(tbn_matrix * (normal_map * 2.0 - 1.0));

    vec3 l = normalize(light_position - surface_position);
    vec3 v = normalize(-surface_position);
    vec3 h = normalize(l + v);

    //  MODIF : Albedo (Base Color)
    vec3 texture_sample_diffuse = pow(texture(albedoMap, surface_texcoord).rgb, vec3(tone_mapping_gamma));
    vec3 albedo = material_color_diffuse * texture_sample_diffuse;

    //  MODIF : armMap = AO (R), Roughness (G), Metallic (B)
    vec3 arm = texture(armMap, surface_texcoord).rgb;

    float ao = material_occlusion * arm.r;
    float roughness = material_roughness * arm.g;
    float metallic = material_metallic * arm.b;

    vec3 ambient = material_color_ambient * albedo * ao;

    float light_distance = length(light_position - surface_position);
    float attenuation = 1.0 / (light_attenuation_factor_constant + light_attenuation_factor_linear * light_distance + light_attenuation_factor_quadratic * light_distance * light_distance);
    vec3 radiance = light_color * attenuation * light_intensity;

    float NdotL = max(dot(n, l), 0.0);
    float NdotV = max(dot(n, v), 0.0);
    float NdotH = max(dot(n, h), 0.0);
    float HdotV = max(dot(h, v), 0.0);

    float d = trowbridge_reitz(n, h, roughness);
    float g = smith(n, l, v, roughness);

    vec3 f0 = mix(material_fresnel_ior, albedo, metallic);
    vec3 f = schlick_fresnel(HdotV, f0);

    vec3 numerator = d * g * f;
    float denominator = 4.0 * max(NdotL * NdotV, 0.001);
    vec3 specular = (numerator / denominator) * material_color_specular;

    vec3 ks = f;
    vec3 kd = (1.0 - ks) * (1.0 - metallic);
    vec3 diffuse = kd * albedo / PI;

    vec3 reflectance = (diffuse + specular) * radiance * NdotL;

    return (ambient + reflectance) * material_brightness;
}

void main()
{
    vec3 color = brdf_cook_torrance();

    color = vec3(1.0) - exp(-color * tone_mapping_exposure);
    color = tone_mapping_toggle ? tone_mapping_aces_filmic(color) : tone_mapping_reinhard(color);
    color = pow(color, vec3(1.0 / tone_mapping_gamma));

    fragment_color = vec4(color, 1.0);

}
