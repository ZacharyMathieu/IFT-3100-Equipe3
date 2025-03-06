#include "scene_controller.h"

void SceneController::setup(int x, int y, int w, int h, GridController* gridController)
{
	SCENE_X = x;
	SCENE_Y = y;
	SCENE_WIDTH = w;
	SCENE_HEIGHT = h;
	wallSize = 1;

	this->gridController = gridController;
	this->ant = gridController->ants[0];

	scale_ant = 0.005f;

	speed = 1.0f;

	box.set(gridController->scaleX * wallSize, wallSize * 5, gridController->scaleY * wallSize);
	box.setPosition(0, 0, 0);
	boxMesh = box.getMesh();

	antSphere.set(wallSize, 120);
	antSphere.setPosition(0, 0, 0);
	vboBoxMeshAnt = antSphere.getMesh();

	antModelLoader.load("models/ant3.obj");
	antModelLoader.setPosition(50, 0, 50);
	antModelLoader.setRotation(0, 90 + 45, 0, 1, 0);
	rotation = antModelLoader.getRotationAngle(0);

	boxCollider = createBoundingBox(antModelLoader);

	antModelLoader.disableMaterials();

	shader_ant.load("lambert_330_vs.glsl", "lambert_330_fs.glsl");
	shader_normal.load("draw_normal_330_vs.glsl", "draw_normal_330_fs.glsl");

	shader = shader_ant;

	cameras.push_back(&mainCamera);
	cameras.push_back(&topCamera);
	cameras.push_back(&freeCamera);
	numCam = 0;
	activeCam = cameras[numCam];

	mainCamera.setPosition(antModelLoader.getPosition().x - 100, 100, antModelLoader.getPosition().z - 100);
	mainCamera.lookAt(ofVec3f(antModelLoader.getPosition()));
	mainCamera.disableMouseInput();

	topCamera.setPosition(antModelLoader.getPosition().x, 500, antModelLoader.getPosition().z);
	topCamera.lookAt(ofVec3f(0, -1, 0));
	topCamera.disableMouseInput();

	freeCamera.setPosition(SCENE_WIDTH / 2, 50, SCENE_HEIGHT / 2);
	freeCamera.lookAt(ofVec3f(0, -1, 0));

	gui.setup();
	checkPop.setName("Vu D'ensemble");
	gui.add(checkPop);
	checkPop = false;
	gui.setPosition(ofGetWidth() - 200, 10);
}

void SceneController::update()
{
	centreX = 3 * (ofGetWidth() / 4.0f);
	centreY = ofGetHeight() / 2.0f;

	antModelLoader.setScale(scale_ant, scale_ant, scale_ant);

	boxCollider.setPosition(antModelLoader.getPosition());
	glm::vec3 newPos = ant->pos;

	if (ofGetKeyPressed(OF_KEY_LEFT)) {
		newPos.x += speed;
		newPos.y -= speed;
		antModelLoader.setRotation(0, rotation - 90, 0, 1, 0);
	}

	if (ofGetKeyPressed(OF_KEY_RIGHT)) {
		newPos.x -= speed;
		newPos.y += speed;
		antModelLoader.setRotation(0, rotation, 0, 1, 0);
		antModelLoader.setRotation(0, rotation + 90, 0, 1, 0);
	}

	if (ofGetKeyPressed(OF_KEY_UP)) {
		newPos.x += speed;
		newPos.y += speed;
		antModelLoader.setRotation(0, rotation, 0, 1, 0);
		if (ofGetKeyPressed(OF_KEY_RIGHT)) {
			antModelLoader.setRotation(0, rotation + 45, 0, 1, 0);
		}
		if (ofGetKeyPressed(OF_KEY_LEFT)) {
			antModelLoader.setRotation(0, rotation - 45, 0, 1, 0);
		}
	}

	if (ofGetKeyPressed(OF_KEY_DOWN)) {
		newPos.x -= speed;
		newPos.y -= speed;
		antModelLoader.setRotation(0, rotation - 180, 0, 1, 0);
		if (ofGetKeyPressed(OF_KEY_RIGHT)) {
			antModelLoader.setRotation(0, rotation + 135, 0, 1, 0);
		}
		if (ofGetKeyPressed(OF_KEY_LEFT)) {
			antModelLoader.setRotation(0, rotation - 135, 0, 1, 0);
		}
	}

	if (!checkCollision(newPos)) {
		antModelLoader.setPosition(newPos.x * gridController->scaleX * wallSize, 0, newPos.y * gridController->scaleY * wallSize);
		ant->pos = newPos;
	}

	mainCamera.setPosition(antModelLoader.getPosition().x - wallSize * 50, wallSize * 50, antModelLoader.getPosition().z - wallSize * 50);
	mainCamera.lookAt(antModelLoader.getPosition());

	topCamera.setPosition(antModelLoader.getPosition().x, wallSize * 100, antModelLoader.getPosition().z);
	//topCamera.setPosition(ant.getPosition().x + 500, 2000, ant.getPosition().z + 500);
	//camera.lookAt(ofVec3f(ant.getPosition().x + 500, ant.getPosition().y, ant.getPosition().z + 500));

	light.setPointLight();
	light.setDiffuseColor(255);
	light.setGlobalPosition(centreX, centreY, 255.0f);
}

void SceneController::draw()
{
	gui.draw();

	int halfHeight = ofGetHeight() / 2;
	int fullWidth = ofGetWidth();

	ofPushView();
	if (checkPop) {
		ofViewport(fullWidth / 2, 50, fullWidth / 2, halfHeight - 50);
	}
	else {
		ofViewport(fullWidth / 2, 50, fullWidth / 2, ofGetHeight());
	}

	ofEnableDepthTest();
	ofEnableLighting();
	light.enable();
	
	activeCam->begin();
	drawScene();
	
	activeCam->end();
	light.disable();
	ofDisableLighting();
	ofDisableDepthTest();
	ofPopView();

	if (checkPop) {
		ofDrawLine(fullWidth / 2, halfHeight, fullWidth, halfHeight);

		ofPushView();
		ofViewport(fullWidth / 2, halfHeight, fullWidth / 2, halfHeight);

		ofEnableDepthTest();
		ofEnableLighting();
		light.enable();

		topCamera.begin();
		drawScene();
		topCamera.end();

		light.disable();
		ofDisableLighting();
		ofDisableDepthTest();
		ofPopView();
	}
}

void SceneController::keyPressed(int key)
{
	if (key == 'c') {
		numCam = (numCam+1)% cameras.size();

		activeCam = cameras[numCam];
	}
	
}

void SceneController::drawScene()
{
	shader.begin();
	shader.setUniform3f("color_ambient", 1, 0, 0);
	shader.setUniform3f("color_diffuse", 0, 0, 1);
	shader.setUniform3f("light_position", light.getGlobalPosition());

	antModelLoader.draw(OF_MESH_FILL);

	shader.end();

	shader.begin();
	shader.setUniform3f("color_ambient", 0, 0, 0);
	shader.setUniform3f("color_diffuse", 1, 1, 1);
	shader.setUniform3f("light_position", light.getGlobalPosition());

	for (auto& pos : antPositions) {
		if (abs(pos.x - antModelLoader.getPosition().x) > 500 && abs(pos.z - antModelLoader.getPosition().z) > 500) continue;
		ofPushMatrix();
		ofTranslate(pos);
		vboBoxMeshAnt.draw();
		ofPopMatrix();
	}
	shader.end();

	for (auto& pos : wallPositions)
	{
		ofPushMatrix();
		ofTranslate(pos);
		ofSetColor(255, 175, 30, 200);
		boxMesh.draw(OF_MESH_WIREFRAME);
		ofPopMatrix();
	}

	ofSetColor(100, 100, 100);

	for (int x = 0; x <= gridController->GRID_WIDTH; x++) {
		ofDrawLine(x * (gridController->scaleX * wallSize), 0, 0, x * (gridController->scaleX * wallSize), 0, SCENE_HEIGHT * wallSize);
	}
	for (int z = 0; z <= gridController->GRID_HEIGHT; z++) {
		ofDrawLine(0, 0, z * (gridController->scaleY * wallSize), SCENE_WIDTH * wallSize, 0, z * (gridController->scaleY * wallSize));
	}
}

ofBoxPrimitive SceneController::createBoundingBox(ofxAssimpModelLoader& model)
{
	ofBoxPrimitive boundingBox;
	boundingBox.set(model.getPosition().x, 50, model.getPosition().z);

	return boundingBox;
}

bool SceneController::checkCollision(glm::vec3 newPos) {
	float halfSize = (wallSize) / 2;
	if (newPos.x < 0 || newPos.x > SCENE_HEIGHT* wallSize || newPos.z < 0 || newPos.z > SCENE_WIDTH * wallSize)
		return true;
	for (auto& pos : wallPositions) {
		if (abs(newPos.x - pos.x) < halfSize * gridController->scaleX &&
			abs(newPos.z - pos.z) < halfSize * gridController->scaleY) {
			return true;
		}
	}

	return false;
}

void SceneController::updateWallPositions() {
	float sizeBoxX = gridController->scaleX * wallSize;
	float sizeBoxY = gridController->scaleY * wallSize;

	wallPositions.clear();

	for (int y = 0; y < gridController->grid.grid.size(); y++) {
		for (int x = 0; x < gridController->grid.grid[y].size(); x++) {
			Cell* cell = gridController->grid.grid[y][x];
			if (cell->type == WALL) {
				if (abs(boxCollider.getPosition().x - ((x * sizeBoxX) + (sizeBoxX / 2))) < (sizeBoxX * 1.5f) / 2
					&& abs(boxCollider.getPosition().z - ((y * sizeBoxY) + (sizeBoxY / 2))) < (sizeBoxY * 1.5) / 2)
					continue;

				glm::vec3 cubePosition((x * sizeBoxX) + (sizeBoxX / 2), wallSize / 2, (y * sizeBoxY) + (sizeBoxY / 2));

				wallPositions.push_back(cubePosition);
			}
		}
	}
}

void SceneController::updateAntPositions() {
	float sizeBoxX = gridController->scaleX * wallSize;
	float sizeBoxY = gridController->scaleY * wallSize;

	antPositions.clear();

	for (Ant* ant : gridController->ants)
	{
		ofPoint posAnt;
		posAnt = ant->pos;
		glm::vec3 antPosition((posAnt.x * sizeBoxX) + (sizeBoxX / 2), 0.5f, (posAnt.y * sizeBoxY) + (sizeBoxY / 2));

		antPositions.push_back(antPosition);
	}
}
