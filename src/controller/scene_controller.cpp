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
	slimes.load("models/slimes.obj");
	slimes.disableMaterials();
	//slimes.setScale(1.5, 1.5f, 1.5f);
	slimesMesh = slimes.getMesh(0);
	ofDisableArbTex;
	wood.load("images/wood.jpg");
	antModelLoader.load("models/ant3.obj");
	ants.load("models/ant3.obj");
	ants.disableMaterials();
	ants.setRotation(0, 180, 0, 1, 0);
	
	boxCollider = createBoundingBox(antModelLoader);

	antModelLoader.disableMaterials();

	
	shader_ant.load("ant_330_vs.glsl", "ant_330_fs.glsl");
	shader_obj.load("obj_330_vs.glsl", "obj_330_fs.glsl");

	shader = shader_obj;

	mainCameraMode = true;
	mainCamera.lookAt(ofVec3f(antModelLoader.getPosition()));
	mainCamera.setScale(-1, 1, 1);
	mainCamera.disableMouseInput();

	topCamera.lookAt(ofVec3f(0, -1, 0));
	topCamera.setScale(0.25, -0.25, 0.25);
	topCamera.enableOrtho();
	topCamera.disableMouseInput();

	popUpCam = &topCamera;
	freeCamera.setPosition(SCENE_WIDTH / 2, 50, SCENE_HEIGHT / 2);
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
	centreX = 3 * (ofGetWidth() / 4.0f);
	centreY = ofGetHeight() / 2.0f;

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
	antModelLoader.setPosition(ant->pos.x * gridController->scaleX * wallSize, 0, ant->pos.y * gridController->scaleY * wallSize);

	mainCamera.setPosition(antModelLoader.getPosition().x, wallSize * 15, antModelLoader.getPosition().z - wallSize * 50);
	mainCamera.lookAt(antModelLoader.getPosition());

	topCamera.setPosition(antModelLoader.getPosition().x, wallSize * 10, antModelLoader.getPosition().z);

	
	float mouseXNormalized = ((ofGetMouseX() / (float)ofGetWidth()) * TWO_PI)/0.5; 
	float mouseYNormalized = ((ofGetMouseY() / (float)ofGetHeight()) * PI)/2;   


	glm::vec3 antPos = antModelLoader.getPosition();
	glm::vec3 lookTarget = antPos + glm::vec3(cos(mouseXNormalized) * 10.0f,
		sin(mouseYNormalized) * 5.0f,
		sin(mouseXNormalized) * 10.0f);

	POV.setPosition(antPos.x, antPos.y +5, antPos.z );

	
	POV.lookAt(lookTarget);

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

	shader_ant.begin();
	shader_ant.setUniform3f("color_ambient", ant->COLOR.r / 255.0f, ant->COLOR.g / 255.0f, ant->COLOR.b / 255.0f);
	shader_ant.setUniform3f("color_diffuse", 1, 1, 1);
	shader_ant.setUniform3f("light_position", light.getGlobalPosition());

	bool visible;

	for (size_t i = 0; i < antPositions.size(); i++)
	{
		if (!objectVisible(antPositions[i], RENDER_DISTANCE_ANTS))
		{
			ofPushMatrix();
			ofTranslate(antPositions[i]);  
			ants.setRotation(0, antAngles[i], 0, 1, 0);

			ants.drawFaces(); 

			ofPopMatrix();
		}
	}


	shader_ant.end();

	shader.begin();
	shader.setUniform3f("color_ambient", 0, 0, 1);
	shader.setUniform3f("color_diffuse", 0, 1, 0);
	shader.setUniform3f("light_position", light.getGlobalPosition());

	glm::vec3 pos;
	Cell* cell;
	int x = 0;
	for (auto& pheromone : pheromonePositions)
	{
		
		pos = get<0>(pheromone);
		cell = get<1>(pheromone);
		float color = pheromoneColorCache[pos];
			x++;
		visible = objectVisible(pos, RENDER_DISTANCE_PHEROMONES);

		if (visible)
			continue;
	
		shader.setUniform3f("translation", pos );
		shader.setUniform1f("scale_factor", cell->getValueFactor()*5);
		slimesMesh.draw();
		//vboPheromone.draw();
		
	}
	shader_ant.end();

	shader.begin();
	shader.setUniform3f("color_ambient", 0, 0, 0);
	shader.setUniform3f("color_diffuse", 0.5f, 0.5f, 0.5f);
	shader.setUniform3f("light_position", light.getGlobalPosition());
	

	for (auto& pos : wallPositions)
	{
		visible = objectVisible(pos, RENDER_DISTANCE_WALLS);

		if (visible)
			continue;

		shader.setUniform3f("translation", pos.x, pos.y, pos.z);
		shader.setUniform1f("scale_factor", 1);
		
		boxMesh.draw(OF_MESH_FILL);
		
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

bool SceneController::objectVisible(glm::vec3 pos, float renderDistance)
{
	return (glm::dot(activeCam->getLookAtDir(), pos - activeCam->getPosition()) < 0) && (glm::distance(activeCam->getPosition(), pos) < renderDistance);
}

void SceneController::updateCellPositions()
{
	float sizeBoxX = gridController->scaleX * wallSize;
	float sizeBoxY = gridController->scaleY * wallSize;
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
				if (abs(boxCollider.getPosition().x - ((x * sizeBoxX) + (sizeBoxX / 2))) < (sizeBoxX * 1.5f) / 2 && abs(boxCollider.getPosition().z - ((y * sizeBoxY) + (sizeBoxY / 2))) < (sizeBoxY * 1.5) / 2)
					continue;

				position = glm::vec3((x * sizeBoxX) + (sizeBoxX / 2), box.getHeight()/2, (y * sizeBoxY) + (sizeBoxY / 2));

				wallPositions.push_back(position);
			}
			else if (cell->type == PHEROMONE && cell->value > 0)
			{

				position = glm::vec3(((x * sizeBoxX) + (sizeBoxX / 2)), 0, (y * sizeBoxY) + (sizeBoxY / 2));

				pheromoneColorCache[position] = ofRandom(0, 1);
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
	ofPoint posAnt;

	antPositions.clear();
	antAngles.clear();

	for (Ant* ant : gridController->ants)
	{
		if (ant != this->ant)
		{
			posAnt = ant->pos;
			position = glm::vec3((posAnt.x * sizeBoxX) + (sizeBoxX / 2), 0.5f, (posAnt.y * sizeBoxY) + (sizeBoxY / 2));

			antPositions.push_back(position);
			antAngles.push_back(ant->a * RAD_TO_DEG + 90);
		}
	}
}
