// IFT3100H25_LambertTeapot/Scene_controller.cpp
// Classe responsable du rendu de l'application.

#include "Scene_controller.h"

void scene_controller::setup(int x, int y, int w, int h)
{
	SCENE_x = x;
	SCENE_y = y;
	SCENE_WIDTH = w;
	SCENE_HEIGHT = h;

	ofSetFrameRate(60);
	
	ofSetLogLevel(OF_LOG_VERBOSE);

	// paramètres
	scale_ant= 0.05f;

	speed = 5.0f;

	box.set(100);
	box.setPosition(0, 0, 0);
	boxMesh = box.getMesh();

	// chargement du modèle
	ant.load("models/ant3.obj");
	ant.setPosition(50, 0, 50);
	ant.setRotation(0, 90 + 45, 0, 1, 0);
	rotation = ant.getRotationAngle(0);

	// désactiver le matériau par défaut du modèle
	ant.disableMaterials();

	// chargement du shader
	shader_ant.load("lambert_330_vs.glsl", "lambert_330_fs.glsl");
	shader_normal.load("draw_normal_330_vs.glsl", "draw_normal_330_fs.glsl");

	// sélectionner le shader courant
	shader = shader_ant;

	cam.setPosition(ant.getPosition().x -100, 100, ant.getPosition().z -100);
	cam.lookAt(ofVec3f(ant.getPosition()));
}

void scene_controller::update()
{
	// position au centre de la fenêtre d'affichage
	centre_x = 3*(ofGetWidth() / 4.0f) ;
	centre_y = ofGetHeight() / 2.0f;

	// transformation du teapot
	ant.setScale(scale_ant, scale_ant, scale_ant);
	
	
	if (ofGetKeyPressed(OF_KEY_LEFT)) {
		ant.setPosition(ant.getPosition().x + speed, ant.getPosition().y, ant.getPosition().z - speed);
		ant.setRotation(0, rotation-90, 0 ,1,0);
		
	}
	if (ofGetKeyPressed(OF_KEY_RIGHT)) {
		ant.setPosition(ant.getPosition().x - speed, ant.getPosition().y, ant.getPosition().z+ speed);
		ant.setRotation(0, rotation, 0, 1, 0);
		ant.setRotation(0, rotation+90, 0, 1, 0);
	}
	if (ofGetKeyPressed(OF_KEY_UP)) {
		ant.setPosition(ant.getPosition().x + speed, ant.getPosition().y, ant.getPosition().z + speed);
		ant.setRotation(0, rotation, 0, 1, 0);
		if (ofGetKeyPressed(OF_KEY_RIGHT)) {
			ant.setRotation(0, rotation+45, 0, 1, 0);
		}
		if (ofGetKeyPressed(OF_KEY_LEFT)) {
			ant.setRotation(0, rotation -45, 0, 1, 0);
		}
	}
	if (ofGetKeyPressed(OF_KEY_DOWN)) {
		ant.setPosition(ant.getPosition().x - speed, ant.getPosition().y, ant.getPosition().z - speed);
		ant.setRotation(0, rotation - 180, 0, 1, 0);
		if (ofGetKeyPressed(OF_KEY_RIGHT)) {
			ant.setRotation(0, rotation +135, 0, 1, 0);
		}
		if (ofGetKeyPressed(OF_KEY_LEFT)) {
			ant.setRotation(0, rotation - 135, 0, 1, 0);
		}
	}
	cam.setPosition(ant.getPosition().x - 100, 100, ant.getPosition().z - 100);
	cam.lookAt(ofVec3f(ant.getPosition()));;
	// configuration de la lumière
	light.setPointLight();
	light.setDiffuseColor(255);
	light.setGlobalPosition(centre_x, centre_y, 255.0f);
}

void scene_controller::draw()
{	

	ofPushMatrix();
	ofViewport(ofRectangle(ofGetWidth() / 2, 50, ofGetWidth()/2, ofGetHeight()));
	
	// couleur de l'arrière-plan
	ofFill();
	ofSetColor(0,0,0,100);
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

	// activer l'occlusion en profondeur
	ofEnableDepthTest();

	// activer l'éclairage dynamique
	ofEnableLighting();

	// activer la lumière
	light.enable();
	cam.begin();
	cam.setFarClip(5000);
	// activer le shader
	shader.begin();
	
	
	// passer les attributs uniformes du shader
	shader.setUniform3f("color_ambient", 1,0,0);
	shader.setUniform3f("color_diffuse", 0,0,1);
	shader.setUniform3f("light_position", light.getGlobalPosition());

	// dessiner le teapot
	ant.draw(OF_MESH_FILL);
	shader.end();

	for (auto& pos : positions)
	{
		ofPushMatrix();
		ofTranslate(pos);
		ofSetColor(255, 175, 30 ,200);
		boxMesh.draw(OF_MESH_WIREFRAME);
		ofPopMatrix();
	}
	// désactiver le shader
	positions.clear();

	ofSetColor(100, 100, 100); // Couleur gris foncé pour le plancher


	int step = 100;
	// Espacement entre les lignes
	ofSetLineWidth(5);

	for (int x = 0; x <= ofGetWidth(); x++) {
		ofDrawLine(x * step, 0, 0, x * step, 0, ofGetHeight() * step);  // Lignes en X
	}

	for (int z = 0; z <= ofGetHeight(); z++) {
		ofDrawLine(0, 0, z * step, ofGetWidth() * step, 0, z * step);  // Lignes en Z
	}
	// désactiver la lumière
	light.disable();

	// désactiver l'éclairage dynamique
	ofDisableLighting();

	// désactiver l'occlusion en profondeur
	ofDisableDepthTest();
	cam.end();
	ofPopMatrix();
}

void scene_controller::keyPressed(int key)
{
	
}
