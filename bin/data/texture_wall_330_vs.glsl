#version 330

// Attributs d'entrée
in vec4 position;   // Position du sommet
in vec3 normal;     // Normale du sommet
in vec2 texcoord;   // Coordonnées de texture

// Attributs en sortie
out vec2 vTexCoord;      // Coordonnées de texture pour le fragment shader
out vec3 surface_normal; // Normale interpolée pour l'éclairage
out vec3 surface_position; // Position interpolée pour l'éclairage

// Matrices et paramètres uniformes
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform vec3 translation;   // Translation de l'objet
uniform float scale_factor; // Facteur d'échelle de l'objet

void main()
{
    // Appliquer la translation et l'échelle
    vec4 transformedPosition = vec4(position.xyz * scale_factor, 1.0) + vec4(translation, 0.0);

    // Transformation normale pour le calcul de la lumière
    mat4 normalMatrix = transpose(inverse(modelViewMatrix));
    surface_normal = normalize(vec3(normalMatrix * vec4(normal, 0.0)));

    // Calculer la position de la surface dans l'espace caméra
    surface_position = vec3(modelViewMatrix * transformedPosition);

    // Passer les coordonnées de texture au fragment shader
    vTexCoord = texcoord;

    // Calculer la position finale du sommet
    gl_Position = projectionMatrix * modelViewMatrix * transformedPosition;
}
