#version 330

// Attributs de sommet
in vec4 position;
in vec4 normal;

// Attributs en sortie
out vec3 surface_position;
out vec3 surface_normal;

// Matrices et paramètres uniformes
uniform mat4 modelMatrix; // Matrice du modèle (Assimp la gère)
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec3 translation;
uniform float scale_factor;

void main()
{
    // Appliquer transformation de modèle + scale + translation
    vec4 transformedPosition = modelMatrix * vec4(position.xyz * scale_factor + translation, 1.0);

    // Matrice normale pour l’éclairage
    mat4 normalMatrix = transpose(inverse(viewMatrix * modelMatrix));
    surface_normal = normalize(vec3(normalMatrix * normal));
    surface_position = vec3(viewMatrix * transformedPosition);

    // Position finale
    gl_Position = projectionMatrix * viewMatrix * transformedPosition;
}
