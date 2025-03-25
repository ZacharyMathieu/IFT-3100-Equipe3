#include "scene_controller.h"

void SceneController::setup(int x, int y, int w, int h, GridController* gridController)
{
	SCENE_X = x;
	SCENE_Y = y;
	SCENE_WIDTH = w;
	SCENE_HEIGHT = h;

	updateGridController(gridController);

	scale_ant = 0.002 * CELL_SIZE;

	box.set(CELL_SIZE, CELL_SIZE, CELL_SIZE);
	boxMesh = box.getMesh();

	antSphere.set(CELL_SIZE / 2, 12);
	vboBoxMeshAnt = antSphere.getMesh();

	cubeMap.load("images/sky.png", 2048, false);

	pheromoneSphere.set(CELL_SIZE / 2, 12);
	vboPheromone = pheromoneSphere.getMesh();
	slimes.load("models/slimes.obj");
	slimes.disableMaterials();
	//slimes.setScale(1.5, 1.5f, 1.5f);
	slimesMesh = slimes.getMesh(0);
	ofDisableArbTex;
	wood.load("images/wood.jpg");
	antModelLoader.load("models/ant3.obj");
	antModelLoader.disableMaterials();

	boxCollider = createBoundingBox(antModelLoader);

	plane = genPlane(gridController->GRID_WIDTH * CELL_SIZE, gridController->GRID_HEIGHT * CELL_SIZE, ofPoint(gridController->GRID_WIDTH / 2 * CELL_SIZE, 0, gridController->GRID_HEIGHT / 2 * CELL_SIZE), ofPoint(0, 1, 0));

	loadShaders();

	//shader = shader_obj;

	mainCamera.lookAt(ofVec3f(antModelLoader.getPosition()));
	mainCamera.setScale(-1, 1, 1);
	//mainCamera.disableMouseInput();

	topCamera.lookAt(ofVec3f(0, -1, 0));
	topCamera.setScale(0.1 * CELL_SIZE, -0.1 * CELL_SIZE, 0.1 * CELL_SIZE);
	topCamera.enableOrtho();
	topCamera.disableMouseInput();

	popUpCam = &topCamera;
	freeCamera.setPosition(0, 0, 0);
	freeCamera.lookAt(ofVec3f(0, -1, 0));


	POV.setNearClip(0.1f);
	POV.disableMouseInput();
	cameras.push_back(&mainCamera);
	cameras.push_back(&topCamera);
	cameras.push_back(&freeCamera);
	cameras.push_back(&POV);
	numCam = 0;
	activeCam = cameras[numCam];

	gui.setup();
	checkPop.setName("Vue D'ensemble");
	gui.add(checkPop);
	checkPop = false;
	gui.setPosition(ofGetWidth() - 200, 10);
}

void SceneController::updateGridController(GridController* gridController)
{
	this->gridController = gridController;
	this->ant = gridController->ants[0];
}

void SceneController::update()
{
	antModelLoader.setScale(scale_ant, scale_ant, scale_ant);
	ants.setScale(scale_ant, scale_ant, scale_ant);


	boxCollider.setPosition(antModelLoader.getPosition());
	ofVec3f newPos = ant->pos;
	float newAngle = ant->a;
	int keysPressed = 0;
	//merci ChatGPT pour quelques conseils
	glm::vec3 lookDir = glm::normalize(POV.getLookAtDir());

	glm::vec3 forward = glm::normalize(glm::vec3(lookDir.x, 0, lookDir.z));
	glm::vec3 right = glm::normalize(glm::vec3(forward.z, 0, -forward.x));

	if (ofGetKeyPressed(OF_KEY_RIGHT) || ofGetKeyPressed(100))
	{
		if (numCam == 3) {
			newPos.x -= right.x * (ANT_MOVE_SPEED * 2);
			newPos.y -= right.z * (ANT_MOVE_SPEED * 2);

		}
		else {
			newPos.x = fmod((newPos.x + (ANT_MOVE_SPEED * 2)), gridController->GRID_WIDTH);
			newAngle = 0;
			keysPressed++;

		}
	}

	if (ofGetKeyPressed(OF_KEY_LEFT) || ofGetKeyPressed(97))
	{
		if (numCam == 3) {
			newPos.x += right.x * (ANT_MOVE_SPEED * 2);
			newPos.y += right.z * (ANT_MOVE_SPEED * 2);

		}
		else {
			newPos.x = fmod((newPos.x - (ANT_MOVE_SPEED * 2) + (float)gridController->GRID_WIDTH), gridController->GRID_WIDTH);
			newAngle = PI;
			keysPressed++;
		}

	}

	if (ofGetKeyPressed(OF_KEY_UP) || ofGetKeyPressed(119))
	{
		if (numCam == 3) {
			newPos.x += forward.x * (ANT_MOVE_SPEED * 2);
			newPos.y += forward.z * (ANT_MOVE_SPEED * 2);

		}
		else {
			newPos.y = fmod((newPos.y + (ANT_MOVE_SPEED * 2)), gridController->GRID_HEIGHT);
			newAngle = HALF_PI;
			keysPressed++;
		}

	}

	if (ofGetKeyPressed(OF_KEY_DOWN) || ofGetKeyPressed(115))
	{
		if (numCam == 3) {
			newPos.x -= forward.x * (ANT_MOVE_SPEED * 2);
			newPos.y -= forward.z * (ANT_MOVE_SPEED * 2);

		}
		else {
			newPos.y = fmod((newPos.y - (ANT_MOVE_SPEED * 2) + gridController->GRID_HEIGHT), gridController->GRID_HEIGHT);
			newAngle = 3 * HALF_PI;
			keysPressed++;
		}
	}


	if (!checkCollision(newPos))
	{
		ant->a = newAngle;
		ant->pos = newPos;
	}

	antModelLoader.setRotation(0, ant->a * RAD_TO_DEG + 90, 0, 1, 0);
	antModelLoader.setPosition(ant->pos.x * CELL_SIZE, 0, ant->pos.y * CELL_SIZE);

	mainCamera.setPosition(antModelLoader.getPosition().x, CELL_SIZE * 5, antModelLoader.getPosition().z - CELL_SIZE * 10);
	mainCamera.lookAt(antModelLoader.getPosition());

	topCamera.setPosition(antModelLoader.getPosition().x, CELL_SIZE * 10, antModelLoader.getPosition().z);
}

void SceneController::draw()
{
	gui.draw();

	int halfHeight = ofGetHeight() / 2;
	int fullWidth = ofGetWidth();

	ofPushView();
	if (checkPop)
	{
		ofViewport(fullWidth / 2, 50, fullWidth / 2, halfHeight - 50);
	}
	else
	{
		ofViewport(fullWidth / 2, 50, fullWidth / 2, ofGetHeight());
	}

	ofEnableDepthTest();
	//ofEnableLighting();
	//light.enable();

	activeCam->begin();
	drawScene();
	activeCam->end();

	//light.disable();
	//ofDisableLighting();
	ofDisableDepthTest();
	ofPopView();

	if (checkPop)
	{
		ofDrawLine(fullWidth / 2, halfHeight, fullWidth, halfHeight);

		ofPushView();
		ofViewport(fullWidth / 2, halfHeight, fullWidth / 2, halfHeight);

		ofEnableDepthTest();
		//ofEnableLighting();
		//light.enable();

		popUpCam->begin();
		drawScene();
		popUpCam->end();

		//light.disable();
		//ofDisableLighting();
		ofDisableDepthTest();
		ofPopView();
	}
}

void SceneController::keyPressed(int key)
{
	switch (key) {
	case 'c':
		numCam = (numCam + 1) % cameras.size();

		activeCam = cameras[numCam];
		break;
	case 'r':
		loadShaders();
		break;
	}
}

void SceneController::drawScene()
{
	cubeMap.draw();
	shader_ant.begin();
	shader_ant.setUniform3f("color_ambient", ant->MAIN_ANT_COLOR.r / 255.0f, ant->MAIN_ANT_COLOR.g / 255.0f, ant->MAIN_ANT_COLOR.b / 255.0f);
	shader_ant.setUniform3f("color_diffuse", 1, 1, 1);
	shader_ant.setUniform3f("light_position", ant->pos.x * CELL_SIZE, CELL_SIZE * 5, ant->pos.y * CELL_SIZE);

	antModelLoader.draw(OF_MESH_FILL);

	shader_ant.end();

	shader_ant.begin();
	shader_ant.setUniform3f("color_ambient", ant->COLOR.r / 255.0f, ant->COLOR.g / 255.0f, ant->COLOR.b / 255.0f);
	shader_ant.setUniform3f("color_diffuse", 1, 1, 1);
	shader_ant.setUniform3f("light_position", light.getGlobalPosition());

	glm::vec3 pos;
	float angle;
	for (auto& data : antData)
	{
		pos = get<0>(data);
		angle = get<1>(data);

		if (objectVisible(pos, RENDER_DISTANCE_ANTS))
		{
			ofPushMatrix();
			ofTranslate(pos);
			ants.setRotation(0, angle, 0, 1, 0);

			ants.drawFaces();

			ofPopMatrix();
		}
	}

	shader_ant.end();

	shader_obj.begin();
	shader_obj.setUniform3f("color_ambient", ant->COLOR.r / 255.0f, ant->COLOR.g / 255.0f, ant->COLOR.b / 255.0f);
	shader_obj.setUniform3f("color_diffuse", 1, 1, 1);
	shader_obj.setUniform3f("light_position", light.getGlobalPosition());

	shader_obj.setUniform4f("color_ambient", 0, 0, 1, 1);
	Cell* cell;
	int x = 0;
	for (auto& pheromone : pheromonePositions)
	{
		pos = get<0>(pheromone);
		cell = get<1>(pheromone);

		if (objectVisible(pos, RENDER_DISTANCE_PHEROMONES)) {
			shader_obj.setUniform3f("translation", pos);
			shader_obj.setUniform1f("scale_factor", cell->getValueFactor());
			vboPheromone.draw();
		}
	}

	//shader.setUniform3f("color_ambient", 0, 0, 0);
	//shader.setUniform3f("color_diffuse", 1, 1, 1);
	//shader.setUniform3f("light_position", light.getGlobalPosition());

	shader_obj.setUniform4f("color_ambient", 0, 0, 0, 1);
	for (auto& pos : wallPositions)
	{
		if (objectVisible(pos, RENDER_DISTANCE_WALLS)) {
			shader_obj.setUniform3f("translation", pos);
			shader_obj.setUniform1f("scale_factor", 1);
			boxMesh.draw(OF_MESH_FILL);
		}
	}

	shader_obj.setUniform3f("translation", 0, 0, 0);
	shader_obj.setUniform1f("scale_factor", 1);
	shader_obj.end();

	ofSetColor(255, 255, 255);
	plane->draw();

	ofSetColor(100, 100, 100);

	for (int x = 0; x <= gridController->GRID_WIDTH; x++)
	{
		ofDrawLine(x * CELL_SIZE, 0, 0, x * CELL_SIZE, 0, GRID_HEIGHT * CELL_SIZE);
	}
	for (int z = 0; z <= gridController->GRID_HEIGHT; z++)
	{
		ofDrawLine(0, 0, z * CELL_SIZE, GRID_WIDTH * CELL_SIZE, 0, z * CELL_SIZE);
	}

}

ofBoxPrimitive SceneController::createBoundingBox(ofxAssimpModelLoader& model)
{
	ofBoxPrimitive boundingBox;
	boundingBox.set(model.getPosition().x, 50, model.getPosition().z);

	return boundingBox;
}

bool SceneController::checkCollision(glm::vec3 newPos)
{
	float halfSize = CELL_SIZE / 2;
	//if (newPos.x < 0 || newPos.x > GRID_HEIGHT * CELL_SIZE || newPos.z < 0 || newPos.z > GRID_WIDTH * CELL_SIZE)
	//	return true;

	for (auto& pos : wallPositions)
	{
		if (abs(newPos.x - pos.x) < halfSize * gridController->scaleX &&
			abs(newPos.z - pos.z) < halfSize * gridController->scaleY)
		{
			return true;
		}
	}

	return false;
}

bool SceneController::objectVisible(glm::vec3 pos, float renderDistance)
{
	return (glm::dot(activeCam->getLookAtDir(), pos - activeCam->getPosition()) > 0) && (glm::distance(activeCam->getPosition(), pos) < renderDistance);
}

void SceneController::updateCellPositions()
{
	//float sizeBoxX = gridController->scaleX * wallSize;
	//float sizeBoxY = gridController->scaleY * wallSize;
	Cell* cell;
	glm::vec3 position;
	ofPoint p;
	int x, y;

	wallPositions.clear();
	pheromonePositions.clear();


	for (y = 0; y < gridController->grid.grid.size(); y++)
	{
		for (x = 0; x < gridController->grid.grid[y].size(); x++)
		{
			cell = gridController->grid.grid[y][x];
			if (cell->type == WALL)
			{
				position = glm::vec3((x * CELL_SIZE) + (CELL_SIZE / 2), box.getHeight() / 2, (y * CELL_SIZE) + (CELL_SIZE / 2));

				wallPositions.push_back(position);
			}
			else if (cell->type == PHEROMONE && cell->value > 0)
			{
				position = glm::vec3((x * CELL_SIZE) + (CELL_SIZE / 2), 0, (y * CELL_SIZE) + (CELL_SIZE / 2));

				pheromoneColorCache[position] = ofRandom(0, 1);
				pheromonePositions.push_back(tuple(position, cell));
			}
		}
	}
}

void SceneController::updateAntPositions()
{
	glm::vec3 position;
	ofPoint posAnt;

	antData.clear();

	for (Ant* _ant : gridController->ants)
	{
		if (_ant != this->ant)
		{
			position = ofPoint(((_ant->pos.x * CELL_SIZE) + (CELL_SIZE / 2)), CELL_SIZE / 2, ((_ant->pos.y * CELL_SIZE) + (CELL_SIZE / 2)));

			antData.push_back(tuple(position, ant->a));
		}
	}
}

ofPlanePrimitive* SceneController::genPlane(float w, float h, ofPoint pos, ofPoint lookAt)
{
	ofPlanePrimitive* plane = new ofPlanePrimitive(w, h, 2, 2);
	plane->lookAt(lookAt);
	plane->setGlobalPosition(pos);
	return plane;
}

void SceneController::loadShaders()
{
	shader_ant.load("ant_330_vs.glsl", "ant_330_fs.glsl");
	shader_obj.load("obj_330_vs.glsl", "obj_330_fs.glsl");
}
