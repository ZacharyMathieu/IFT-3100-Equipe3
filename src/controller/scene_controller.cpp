#include "scene_controller.h"
#include <ofGraphics.h>

void scene_controller::setup(int x, int y, int w, int h)
{
	
	SCENE_x = x;
	SCENE_y = y;
	SCENE_WIDTH = w;
	SCENE_HEIGHT = h;

	ofSetFrameRate(60);
	ofSetBackgroundColor(240, 240, 240);
	scale_ant = 0.5f;

	shader_ant.load("lambert_330_vs.glsl", "lambert_330_fs.glsl");
	
	shader = shader_ant;
	sceneView.set(ofGetWidth() / 2, 50, ofGetWidth() / 2, ofGetHeight() - 50);
	//box.set(100);
	ant.loadModel("models/ant3.obj");
	
	cam.setPosition(0,1250, 2000);
	cam.lookAt(ofVec3f(0, 0, 0));
	ofAddListener(ofEvents().mouseDragged, this, &scene_controller::mouseDragged);
}

void scene_controller::update()
{
	centre_x = SCENE_WIDTH / 2.0f;
	centre_y = SCENE_HEIGHT / 2.0f;

	ant.setScale(scale_ant, scale_ant, scale_ant);
	ant.setPosition(centre_x, centre_y, 0);

	light.setPointLight();
	light.setDiffuseColor(ofColor(255,255,255));
	light.setGlobalPosition(centre_x, centre_y, 255.0f);
	

}

void scene_controller::draw()
{	
	ofViewport(sceneView);
	cam.begin();
	
	ofEnableDepthTest();
	ofEnableLighting();
	light.enable();
	shader.begin();

	shader.setUniform3f("color_ambient", 1.0f, 0.0f, 0.0f);
	shader.setUniform3f("color_diffuse", 0.0f, 0.0f, 1.0f);
	shader.setUniform3f("light_position", light.getGlobalPosition());

	ant.draw(OF_MESH_FILL);
	
	shader.end();

	ofSetColor(100, 100, 100,100); // Couleur gris foncé pour le plancher

	int gridSize = SCENE_WIDTH*10; // Taille de la grille
	int step = 50;      // Espacement entre les lignes

	for (int x = -gridSize; x <= gridSize; x += step) {
		ofDrawLine(x, 0, -gridSize, x, 0, gridSize);  // Lignes en X
	}

	for (int z = -gridSize; z <= gridSize; z += step) {
		ofDrawLine(-gridSize, 0, z, gridSize, 0, z);  // Lignes en Z
	}
	
	cam.end();
	light.disable();
	ofDisableLighting();
	ofDisableDepthTest();
	
	
	

}
void scene_controller::mouseDragged(ofMouseEventArgs& args)
{
	// Récupérer la position actuelle de la caméra
	ofVec3f camPos = cam.getPosition();

	// Définir une hauteur minimale
	float minY = 10.0f;

	// Si la caméra passe sous `minY`, la forcer à rester au-dessus
	if (camPos.y < minY) {
		camPos.y = minY;
		cam.setPosition(camPos);
	}
}
