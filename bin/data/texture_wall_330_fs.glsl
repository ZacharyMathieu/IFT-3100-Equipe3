#version 330

// Uniformes
uniform sampler2D texture0;    // Texture appliquée
uniform vec3 light_position;   // Position de la source de lumière
uniform vec3 color_ambient;    // Couleur ambiante du matériau
uniform vec3 color_diffuse;    // Couleur diffuse du matériau

// Attributs d'entrée depuis le Vertex Shader
in vec2 vTexCoord;        // Coordonnées de texture
in vec3 surface_normal;   // Normale interpolée
in vec3 surface_position; // Position interpolée

// Sortie
out vec4 fragment_color;

void main()
{
    // Re-normaliser la normale après interpolation
    vec3 n = normalize(surface_normal);

    // Calculer la direction de la lumière (L)
    vec3 l = normalize(light_position - surface_position);

    // Calculer le niveau de réflexion diffuse (Lambert)
    float reflection_diffuse = max(dot(n, l), 0.0);

    // Récupérer la couleur de la texture
    vec4 texColor = texture(texture0, vTexCoord);

    // Combiner texture et lumière
    vec3 finalColor = (color_ambient + color_diffuse * reflection_diffuse) * texColor.rgb;

    // Définir la couleur du fragment
    fragment_color = vec4(finalColor, texColor.a);
}
 