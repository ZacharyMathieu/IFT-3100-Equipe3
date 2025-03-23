#version 330

// Attributs de sommet
in vec4 position;
in vec4 normal;
in vec2 texcoord;

// Attributs en sortie
out vec3 surface_position;
out vec3 surface_normal;


// Matrices et paramètres uniformes
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform vec3 translation;  // Position de l’objet
uniform float scale_factor; // Échelle de l’objet

void main()
{
	
  // Appliquer la translation et l'échelle
  vec4 transformedPosition = vec4(position.xyz * scale_factor, 1.0) + vec4(translation, 0.0);

  // Transformation normale et éclairage
  mat4 normalMatrix = transpose(inverse(modelViewMatrix));
  surface_normal = vec3(normalMatrix * normal);
  surface_position = vec3(modelViewMatrix * transformedPosition);

  // Position finale du sommet
  gl_Position = projectionMatrix * modelViewMatrix * transformedPosition;
}