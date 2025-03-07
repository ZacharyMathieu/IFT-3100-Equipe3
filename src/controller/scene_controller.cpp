#include "scene_controller.h"

void SceneController::setup(int x, int y, int w, int h, GridController* gridController)
{
	SCENE_X = x;
	SCENE_Y = y;
	SCENE_WIDTH = w;
	SCENE_HEIGHT = h;
	wallSize = 1;

	updateGridController(gridController);

	scale_ant = 0.005f;

	box.set(gridController->scaleX * wallSize, wallSize * 5, gridController->scaleY * wallSize);
	boxMesh = box.getMesh();

	antSphere.set(wallSize, 120);
	vboBoxMeshAnt = antSphere.getMesh();

	pheromoneSphere.set(wallSize, 12);
	vboPheromone = pheromoneSphere.getMesh();

	antModelLoader.load("models/ant3.obj");

	boxCollider = createBoundingBox(antModelLoader);

	antModelLoader.disableMaterials();

	shader_ant.load("ant_330_vs.glsl", "ant_330_fs.glsl");
	shader_obj.load("obj_330_vs.glsl", "obj_330_fs.glsl");

	shader = shader_obj;

	mainCameraMode = true;
	mainCamera.lookAt(ofVec3f(antModelLoader.getPosition()));
	mainCamera.setScale(-1, 1, 1);
	mainCamera.disableMouseInput();
	mainCamera.setFarClip(150);

	topCamera.lookAt(ofVec3f(0, -1, 0));
	topCamera.setScale(0.25, 0.25, -0.25);
	topCamera.enableOrtho();
	topCamera.disableMouseInput();

	popUpCam = &topCamera;
	freeCamera.setPosition(SCENE_WIDTH / 2, 50, SCENE_HEIGHT / 2);
	freeCamera.lookAt(ofVec3f(0, -1, 0));

	cameras.push_back(&mainCamera);
	cameras.push_back(&topCamera);
	cameras.push_back(&freeCamera);
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
	centreX = 3 * (ofGetWidth() / 4.0f);
	centreY = ofGetHeight() / 2.0f;

	antModelLoader.setScale(scale_ant, scale_ant, scale_ant);

	boxCollider.setPosition(antModelLoader.getPosition());
	ofVec3f newPos = ant->pos;
	float newAngle = ant->a;
	int keysPressed = 0;

	if (ofGetKeyPressed(OF_KEY_RIGHT))
	{
		newPos.x = fmod((newPos.x + (ANT_MOVE_SPEED * 2)), gridController->GRID_WIDTH);
		newAngle = 0;
		keysPressed++;
	}

	if (ofGetKeyPressed(OF_KEY_LEFT))
	{
		newPos.x = fmod((newPos.x - (ANT_MOVE_SPEED * 2) + (float)gridController->GRID_WIDTH), gridController->GRID_WIDTH);
		newAngle = PI;
		keysPressed++;
	}

	if (ofGetKeyPressed(OF_KEY_UP))
	{
		newPos.y = fmod((newPos.y + (ANT_MOVE_SPEED * 2)), gridController->GRID_HEIGHT);
		newAngle = HALF_PI;
		keysPressed++;
	}

	if (ofGetKeyPressed(OF_KEY_DOWN))
	{
		newPos.y = fmod((newPos.y - (ANT_MOVE_SPEED * 2) + gridController->GRID_HEIGHT), gridController->GRID_HEIGHT);
		newAngle = 3 * HALF_PI;
		keysPressed++;
	}

	if (!checkCollision(newPos))
	{
		ant->a = newAngle;
		ant->pos = newPos;
	}

	antModelLoader.setRotation(0, ant->a * RAD_TO_DEG + 90, 0, 1, 0);
	antModelLoader.setPosition(ant->pos.x * gridController->scaleX * wallSize, 0, ant->pos.y * gridController->scaleY * wallSize);

	mainCamera.setPosition(antModelLoader.getPosition().x, wallSize * 15, antModelLoader.getPosition().z - wallSize * 50);
	mainCamera.lookAt(antModelLoader.getPosition());

	topCamera.setPosition(antModelLoader.getPosition().x, wallSize * 10, antModelLoader.getPosition().z);

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
	if (checkPop)
	{
		ofViewport(fullWidth / 2, 50, fullWidth / 2, halfHeight - 50);
	}
	else
	{
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

	if (checkPop)
	{
		ofDrawLine(fullWidth / 2, halfHeight, fullWidth, halfHeight);

		ofPushView();
		ofViewport(fullWidth / 2, halfHeight, fullWidth / 2, halfHeight);

		ofEnableDepthTest();
		ofEnableLighting();
		light.enable();

		popUpCam->begin();
		drawScene();
		popUpCam->end();

		light.disable();
		ofDisableLighting();
		ofDisableDepthTest();
		ofPopView();
	}
}

void SceneController::keyPressed(int key)
{
	if (key == 'c')
	{
		numCam = (numCam + 1) % cameras.size();

		activeCam = cameras[numCam];
	}
}

void SceneController::drawScene()
{

	shader_ant.begin();
	shader_ant.setUniform3f("color_ambient", ant->MAIN_ANT_COLOR.r / 255.0f, ant->MAIN_ANT_COLOR.g / 255.0f, ant->MAIN_ANT_COLOR.b / 255.0f);
	shader_ant.setUniform3f("color_diffuse", 1, 1, 1);
	shader_ant.setUniform3f("light_position", light.getGlobalPosition());

	antModelLoader.draw(OF_MESH_FILL);

	shader_ant.end();

	shader.begin();
	shader.setUniform3f("color_ambient", ant->COLOR.r / 255.0f, ant->COLOR.g / 255.0f, ant->COLOR.b / 255.0f);
	shader.setUniform3f("color_diffuse", 1, 1, 1);
	shader.setUniform3f("light_position", light.getGlobalPosition());

	for (auto& pos : antPositions)
	{
		vector<bool> visible = objectBehindCam(pos, RENDER_DISTANCE_ANTS);

		if (!checkPop)
		{
			if (visible[0])
				continue;
		}
		else
		{
			if (visible[0] && visible[1])
				continue;
		}

		shader.setUniform3f("translation", pos.x, pos.y, pos.z);
		shader.setUniform1f("scale_factor", 1);

		vboBoxMeshAnt.draw();
	}

	shader.end();

	shader.begin();
	shader.setUniform3f("color_ambient", 0, 0, 1);
	shader.setUniform3f("color_diffuse", 1, 1, 1);
	shader.setUniform3f("light_position", light.getGlobalPosition());

	glm::vec3 pos;
	Cell* cell;
	//float scale;
	float sizeBoxX = gridController->scaleX * wallSize;
	float sizeBoxY = gridController->scaleY * wallSize;
	for (auto& pheromone : pheromonePositions)
	{
		pos = get<0>(pheromone);
		cell = get<1>(pheromone);

		vector<bool> visible = objectBehindCam(pos, RENDER_DISTANCE_PHEROMONES);
		if (!checkPop)
		{
			if (visible[0]) continue;
		}
		else
		{
			if (visible[0] && visible[1]) continue;
		}

		shader.setUniform3f("translation", pos);
		shader.setUniform1f("scale_factor", cell->getValueFactor());
		vboPheromone.draw();
		//drawObj(pos);
	}
	shader.end();

	shader.begin();
	shader.setUniform3f("color_ambient", 0, 1, 1);
	shader.setUniform3f("color_diffuse", 0, 1, 1);
	shader.setUniform3f("light_position", light.getGlobalPosition());

	for (auto& pos : wallPositions)
	{
		vector<bool> visible = objectBehindCam(pos, RENDER_DISTANCE_WALLS);
		if (!checkPop) {
			if (visible[0]) continue;
		}
		else {
			if (visible[0] && visible[1])continue;
		}
		shader.setUniform3f("translation", pos.x, pos.y, pos.z);
		shader.setUniform1f("scale_factor", 1);
		boxMesh.draw(OF_MESH_WIREFRAME);
	}
	shader.end();
	ofSetColor(100, 100, 100);

	for (int x = 0; x <= gridController->GRID_WIDTH; x++)
	{
		ofDrawLine(x * (gridController->scaleX * wallSize), 0, 0, x * (gridController->scaleX * wallSize), 0, SCENE_HEIGHT * wallSize);
	}
	for (int z = 0; z <= gridController->GRID_HEIGHT; z++)
	{
		ofDrawLine(0, 0, z * (gridController->scaleY * wallSize), SCENE_WIDTH * wallSize, 0, z * (gridController->scaleY * wallSize));
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
	float halfSize = (wallSize) / 2;
	if (newPos.x < 0 || newPos.x > SCENE_HEIGHT * wallSize || newPos.z < 0 || newPos.z > SCENE_WIDTH * wallSize)
		return true;
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

float SceneController::conversionColorToScale(ofColor color)
{
	// float scale = 1.0f - (cell->getCellColor().b + cell->getCellColor().r + cell->getCellColor().g) / (255.0f * 3.0f);

	return ofClamp(1, 0, 1.0f);
}

ofPoint SceneController::conversionGridToPixel(float x, float y)
{
	float sizeBoxX = gridController->scaleX * wallSize;
	float sizeBoxY = gridController->scaleY * wallSize;

	int a = round((x - (sizeBoxX / 2)) / sizeBoxX);
	int b = round((y - (sizeBoxY / 2)) / sizeBoxY);

	ofPoint p = ofPoint(a, b);

	// return p;
	return ofPoint(0);
}

vector<bool> SceneController::objectBehindCam(glm::vec3 pos, int dist)
{
	bool active = false;
	bool checkPopCam = false;
	vector<bool> objBehind;

	if (glm::distance(pos, activeCam->getPosition()) > dist) active = true;
	if (glm::distance(pos, popUpCam->getPosition()) > dist) checkPopCam = true;

	// R�cup�rer la direction de la cam�ra
	glm::vec3 camDirection = activeCam->getLookAtDir();
	glm::vec3 camDirection2 = popUpCam->getLookAtDir();

	glm::vec3 toObject = glm::normalize(pos - activeCam->getPosition());
	glm::vec3 toObject2 = glm::normalize(pos - popUpCam->getPosition());

	if (glm::dot(camDirection, toObject) < 0) active = true;
	if (glm::dot(camDirection, toObject2) < 0) checkPopCam = true;

	objBehind.push_back(active);
	objBehind.push_back(checkPopCam);

	return objBehind;
}

void SceneController::updateCellPositions()
{
	float sizeBoxX = gridController->scaleX * wallSize;
	float sizeBoxY = gridController->scaleY * wallSize;
	Cell* cell;
	glm::vec3 position;
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
				if (abs(boxCollider.getPosition().x - ((x * sizeBoxX) + (sizeBoxX / 2))) < (sizeBoxX * 1.5f) / 2 && abs(boxCollider.getPosition().z - ((y * sizeBoxY) + (sizeBoxY / 2))) < (sizeBoxY * 1.5) / 2)
					continue;

				position = glm::vec3((x * sizeBoxX) + (sizeBoxX / 2), 0, (y * sizeBoxY) + (sizeBoxY / 2));

				wallPositions.push_back(position);
			}
			else
			{
				position = glm::vec3((x * sizeBoxX) + (sizeBoxX / 2), 0, (y * sizeBoxY) + (sizeBoxY / 2));

				pheromonePositions.push_back(tuple(position, cell));
			}
		}
	}
}

void SceneController::updateAntPositions()
{
	float sizeBoxX = gridController->scaleX * wallSize;
	float sizeBoxY = gridController->scaleY * wallSize;
	glm::vec3 position;

	antPositions.clear();

	for (Ant* ant : gridController->ants)
	{
		if (ant != this->ant)
		{
			ofPoint posAnt;
			posAnt = ant->pos;
			glm::vec3 antPosition((posAnt.x * sizeBoxX) + (sizeBoxX / 2), 0.5f, (posAnt.y * sizeBoxY) + (sizeBoxY / 2));

			antPositions.push_back(position);
		}
	}
}
