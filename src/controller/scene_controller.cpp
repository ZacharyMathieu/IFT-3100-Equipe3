// IFT3100H25_LambertTeapot/Scene_controller.cpp
// Classe responsable du rendu de l'application.

#include "scene_controller.h"

void SceneController::setup(int x, int y, int w, int h)
{
	SCENE_x = x;
	SCENE_y = y;
	SCENE_WIDTH = w;
	SCENE_HEIGHT = h;
	wallSize = 50;
	ofSetFrameRate(60);
	
	ofSetLogLevel(OF_LOG_VERBOSE);
	
	
	// param�tres
	scale_ant= 0.05f;

	speed = 5.0f;

	box.set(wallSize);
	box.setPosition(0, 0, 0);
	boxMesh = box.getMesh();

	// chargement du mod�le
	ant.load("models/ant3.obj");
	ant.setPosition(50, 0, 50);
	ant.setRotation(0, 90 + 45, 0, 1, 0);
	rotation = ant.getRotationAngle(0);

	// d�sactiver le mat�riau par d�faut du mod�le
	ant.disableMaterials();

	// chargement du shader
	shader_ant.load("lambert_330_vs.glsl", "lambert_330_fs.glsl");
	shader_normal.load("draw_normal_330_vs.glsl", "draw_normal_330_fs.glsl");

	// s�lectionner le shader courant
	shader = shader_ant;

	cam.setPosition(ant.getPosition().x -100, 100, ant.getPosition().z -100);
	cam.lookAt(ofVec3f(ant.getPosition()));
	mainCamera = true;
	camera.setPosition(500, 700, 500);
	camera.lookAt(ofVec3f(ant.getPosition().x +500, ant.getPosition().y, ant.getPosition().z +500 ));
	popupCam = camera;

	gui.setup();
	//openPopup.setup("Vu d'ensemble");
	checkPop.setName("Vu D'ensemble");
	gui.add(checkPop);
	checkPop = false;

	//openPopup.addListener(this, &SceneController::drawSecondWindow);
	//gui.add(&openPopup);
	gui.setPosition(ofGetWidth() - 200, 10);
}

void SceneController::update()
{
	// position au centre de la fen�tre d'affichage
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
	camera.setPosition(ant.getPosition().x + 500, 2000, ant.getPosition().z + 500);
	camera.lookAt(ofVec3f(ant.getPosition().x + 500, ant.getPosition().y, ant.getPosition().z + 500));
	
	light.setPointLight();
	light.setDiffuseColor(255);
	light.setGlobalPosition(centre_x, centre_y, 255.0f);
}

void SceneController::draw()
{	
	gui.draw();

	
	int halfHeight = ofGetHeight() / 2;
	int fullWidth = ofGetWidth();
	ofSetColor(0);
	ofSetLineWidth(7);
	ofDrawLine(fullWidth / 2, 0, fullWidth / 2, halfHeight * 2);
	
	ofPushView();
	if (checkPop) {
		ofViewport(fullWidth/2, 50, fullWidth/2, halfHeight-50); 
	}
	else {
		ofViewport(fullWidth / 2, 50, fullWidth/2, ofGetHeight());
	}

	ofEnableDepthTest();
	ofEnableLighting();
	light.enable();
	if (mainCamera) {
		cam.begin();
	}
	else {
		camera.begin();
	}
	
	drawScene();
	if (mainCamera) {
		cam.end();
	}
	else {
		camera.end();
	}

	light.disable();
	ofDisableLighting();
	ofDisableDepthTest();
	ofPopView();

	
	if (checkPop) {
		ofDrawLine(fullWidth / 2, halfHeight, fullWidth, halfHeight);
		ofPushView();
		ofViewport(fullWidth/2, halfHeight+50, fullWidth/2, halfHeight);  

		ofEnableDepthTest();
		ofEnableLighting();
		light.enable();

		camera.begin();  
		drawScene();
		camera.end();

		light.disable();
		ofDisableLighting();
		ofDisableDepthTest();
		ofPopView();
	}
	
	
}

void SceneController::keyPressed(int key)
{
	if (key == 'c') {
		
		mainCamera = !mainCamera;
	}
}

void SceneController::drawSecondWindow()
{
	showPopup = !showPopup;
	ofLog() << "appel de deuxième fenetre" << (showPopup ? "on" : "off");
}

void SceneController::drawScene()
{
	shader.begin();
	shader.setUniform3f("color_ambient", 1, 0, 0);
	shader.setUniform3f("color_diffuse", 0, 0, 1);
	shader.setUniform3f("light_position", light.getGlobalPosition());

	ant.draw(OF_MESH_FILL);
	shader.end();

	for (auto& pos : positions)
	{
		ofPushMatrix();
		ofTranslate(pos);
		ofSetColor(255, 175, 30, 200);
		boxMesh.draw(OF_MESH_WIREFRAME);
		ofPopMatrix();
	}

	ofSetColor(100, 100, 100);
	int step = 50;

	for (int x = 0; x <= ofGetWidth(); x++) {
		ofDrawLine(x * step, 0, 0, x * step, 0, ofGetHeight() * step);
	}
	for (int z = 0; z <= ofGetHeight(); z++) {
		ofDrawLine(0, 0, z * step, ofGetWidth() * step, 0, z * step);
	}
}

