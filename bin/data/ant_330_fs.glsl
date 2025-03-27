#version 330

in vec2 v_texcoord;
in vec3 v_normal;

uniform sampler2D texture0;

uniform vec3 color_ambient;
uniform vec3 color_diffuse;
uniform vec3 color_specular;
uniform vec3 light_position;

uniform float brightness;

out vec4 fragment_color;

void main() {
    // Normale et direction de la lumière
    vec3 normal = normalize(v_normal);
    vec3 lightDir = normalize(light_position); // suppose lumière directionnelle

    // Calcul de l'intensité diffuse
    float diff = max(dot(normal, lightDir), 0.0);

    // Couleur de texture
    vec3 texColor = texture(texture0, v_texcoord).rgb;

    // Résultat final (pas de spéculaire ici, mais tu peux l'ajouter)
    vec3 finalColor = (color_ambient  +
                      color_diffuse * diff * brightness) * texColor ;

    fragment_color = vec4(finalColor, 1.0);
}
