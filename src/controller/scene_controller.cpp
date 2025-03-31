#include "scene_controller.h"

void SceneController::setup(int x, int y, int w, int h, GridController* gridController)
{
	ofEnableDepthTest();
	ofDisableArbTex();

	SCENE_X = x;
	SCENE_Y = y;
	SCENE_WIDTH = w;
	SCENE_HEIGHT = h;
	boxSize = 1;

	updateGridController(gridController);

	scale_ant = 0.002f * boxSize;

	box.set(boxSize, boxSize * 5, boxSize);
	box.mapTexCoords(0, 0, 2, 2);
	boxMesh = box.getMesh();

	antSphere.set(boxSize, 64);
	vboBoxMeshAnt = antSphere.getMesh();

	//cubeMap.load("images/sky.png", 2048, false);

	pheromoneSphere.set(boxSize, 12);
	vboPheromone = pheromoneSphere.getMesh();
	slimes.load("models/slimes.obj");
	slimes.disableMaterials();

	slimesMesh = slimes.getMesh(0);

	//crackWall.load("images/crackWall.jpg");
	//glitter.load("images/glitter.jpg");
	//fire.load("images/fire.jpg");
	//wood.load("images/wood.jpg");
	//rock.load("images/rock.jpg");
	//paint.load("images/paint.jpg");
	//antTexture.load("models/antTexture.jpg");

	//wallTextures.push_back(wood.getTexture());
	//wallTextures.push_back(crackWall.getTexture());
	//wallTextures.push_back(rock.getTexture());
	//wallTextures.push_back(paint.getTexture());
	//wallTextures.push_back(glitter.getTexture());
	//wallTextures.push_back(fire.getTexture());

	//texture.setTextureMinMagFilter(GL_LINEAR, GL_LINEAR);
	//texture.setTextureWrap(GL_REPEAT, GL_REPEAT);

	antModelLoader.load("models/ant3.obj");
	antModelLoader.disableMaterials();

	//albedo.load("models/newAnt3/Textures/material_baseColor.jpg");
	//normalMap.load("models/newAnt3/Textures/material_normal.png");
	//metallicRoughnessMap.load("models/newAnt3/Textures/material_metallicRoughness.png");

	//a = albedo.getTexture();
	//n = normalMap.getTexture();
	//m = metallicRoughnessMap.getTexture();
	//a.setTextureMinMagFilter(GL_LINEAR, GL_LINEAR);
	//n.setTextureMinMagFilter(GL_LINEAR, GL_LINEAR);
	//m.setTextureMinMagFilter(GL_LINEAR, GL_LINEAR);

	ants.load("models/ant3.obj");

	ants.disableMaterials();
	ants.setRotation(0, 180, 0, 1, 0);

	boxCollider = createBoundingBox(antModelLoader);

	shader_ant.load("ant_330_vs.glsl", "ant_330_fs.glsl");
	shader_obj.load("obj_330_vs.glsl", "obj_330_fs.glsl");
	shader_texture_wall.load("texture_wall_330_vs.glsl", "texture_wall_330_fs.glsl");

	shader = shader_obj;

	mainCameraMode = true;
	mainCamera.lookAt(ofVec3f(antModelLoader.getPosition()));
	mainCamera.setScale(-boxSize, boxSize, boxSize);
	mainCamera.disableMouseInput();

	topCamera.lookAt(ofVec3f(0, -1, 0));
	topCamera.setScale(0.25 * boxSize, -0.25 * boxSize, 0.25 * boxSize);
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

	if (ofGetKeyPressed(OF_KEY_RIGHT) || ofGetKeyPressed('d'))
	{
		if (numCam == 3) {
			newPos.x -= right.x * (ANT_MOVE_SPEED * 2);
			newPos.y -= right.z * (ANT_MOVE_SPEED * 2);
		}
		else
		{
			newPos.x = fmod((newPos.x + (ANT_MOVE_SPEED * 2)), gridController->GRID_WIDTH);
			newAngle = 0;
			keysPressed++;
		}
	}

	if (ofGetKeyPressed(OF_KEY_LEFT) || ofGetKeyPressed('a'))
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

	if (ofGetKeyPressed(OF_KEY_UP) || ofGetKeyPressed('w'))
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

	if (ofGetKeyPressed(OF_KEY_DOWN) || ofGetKeyPressed('s'))
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
	antModelLoader.setPosition(ant->pos.x * boxSize, 0, ant->pos.y * boxSize);

	mainCamera.setPosition(antModelLoader.getPosition().x, boxSize * 5, antModelLoader.getPosition().z - boxSize * 10);

	mainCamera.lookAt(antModelLoader.getPosition());

	topCamera.setPosition(antModelLoader.getPosition().x, boxSize * 10, antModelLoader.getPosition().z);

	float mouseXNormalized = ((ofGetMouseX() / (float)ofGetWidth()) * TWO_PI) / 0.5;
	float mouseYNormalized = ((ofGetMouseY() / (float)ofGetHeight()) * PI) / 2;


	glm::vec3 antPos = antModelLoader.getPosition();
	glm::vec3 lookTarget = antPos + glm::vec3(cos(mouseXNormalized) * 10.0f,
		sin(mouseYNormalized) * 5.0f,
		sin(mouseXNormalized) * 10.0f);

	POV.setPosition(antPos.x, antPos.y + 5, antPos.z);


	POV.lookAt(lookTarget);

	light.setDirectional();
	light.setDiffuseColor(ofColor(31, 255, 31));
	light.setSpecularColor(ofColor(191, 191, 191));
	light.setOrientation(ofVec3f(-45, 60, 0));


	light.setGlobalPosition(centreX, centreY, 255.0f);
	texture.setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
	texture.setTextureWrap(GL_REPEAT, GL_REPEAT);
	textureAnt.setTextureMinMagFilter(GL_LINEAR, GL_LINEAR);
	textureAnt.setTextureWrap(GL_REPEAT, GL_REPEAT);
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
	cubeMap.draw();

	shader_ant.begin();
	//shader_ant.setUniformTexture("texture0", glitter.getTexture(), 0);
	shader_ant.setUniform3f("light_position", light.getGlobalPosition());
	shader_ant.setUniform3f("color_ambient", ant->MAIN_ANT_COLOR.r / 255, ant->MAIN_ANT_COLOR.g / 255, ant->MAIN_ANT_COLOR.b / 255);
	shader_ant.setUniform3f("color_diffuse", light.getDiffuseColor().r / 255, light.getDiffuseColor().g / 255, light.getDiffuseColor().b / 255);
	shader_ant.setUniform3f("color_specular", light.getSpecularColor().r / 255, light.getSpecularColor().g / 255, light.getSpecularColor().b / 255);
	shader_ant.setUniform1f("brightness", 2);

	antModelLoader.drawFaces();

	shader_ant.setUniform3f("color_ambient", ant->COLOR.r / 255, ant->COLOR.g / 255, ant->COLOR.b / 255);

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

		if (objectVisible(pos, RENDER_DISTANCE_PHEROMONES))
			continue;

		shader.setUniform3f("translation", pos.x + ofRandom(-1, 1), pos.y, pos.z + ofRandom(-1, 1));
		shader.setUniform1f("scale_factor", cell->getValueFactor() * +ofRandom(2, 5));
		slimesMesh.draw();

		//*****Décommenter si on veut ajouter des pheromones autour.***

		/*shader.setUniform3f("translation", pos.x +ofRandom(-3,3), pos.y, pos.z + ofRandom(-3, 3));
		shader.setUniform1f("scale_factor", cell->getValueFactor() * +ofRandom(1, 4));
		slimesMesh.draw();
		shader.setUniform3f("translation", pos.x + ofRandom(-3, 3), pos.y, pos.z + ofRandom(-3, 3));
		shader.setUniform1f("scale_factor", cell->getValueFactor() * +ofRandom(1, 4));
		slimesMesh.draw();
		vboPheromone.draw();*/

	}
	shader.end();

	shader_texture_wall.begin();
	shader_texture_wall.setUniformTexture("texture0", texture, 0);
	shader_texture_wall.setUniform3f("color_ambient", 0.5f, 0.5f, 0.5f);
	shader_texture_wall.setUniform3f("color_diffuse", 1, 1, 1);
	shader_texture_wall.setUniform3f("light_position", light.getGlobalPosition());
	for (auto& pos : wallPositions)
	{
		if (objectVisible(pos, RENDER_DISTANCE_WALLS))
			continue;

		shader_texture_wall.setUniform3f("translation", pos.x, pos.y, pos.z);
		shader_texture_wall.setUniform1f("scale_factor", 1);
		float maxAniso;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
		texture.bind();
		boxMesh.draw(OF_MESH_FILL);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAniso);
		texture.unbind();

	}

	shader_texture_wall.end();

	ofFill();
	ofSetLineWidth(10);
	ofSetColor(250);

	ofPushMatrix();
	ofTranslate(0, 0, 0);
	ofRotateDeg(-90, 1, 0, 0);
	ofDrawRectangle(0, 0, SCENE_WIDTH * boxSize, -SCENE_HEIGHT * boxSize);
	ofPopMatrix();
	for (int x = 0; x <= gridController->GRID_WIDTH; x++)
	{
		ofSetColor(0);
		ofDrawLine(x * (boxSize), 0, 0, x * (boxSize), 0, gridController->GRID_HEIGHT * boxSize);
	}
	for (int z = 0; z <= gridController->GRID_HEIGHT; z++)
	{
		ofSetColor(0);
		ofDrawLine(0, 0, z * (boxSize), gridController->GRID_WIDTH * boxSize, 0, z * (boxSize));
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
	float halfSize = (boxSize) / 2;
	if (newPos.x < 0 || newPos.x > SCENE_HEIGHT * boxSize || newPos.z < 0 || newPos.z > SCENE_WIDTH * boxSize)
		return true;

	for (auto& pos : wallPositions)
	{
		if (abs(newPos.x - pos.x) < halfSize &&
			abs(newPos.z - pos.z) < halfSize)
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
	float sizeBoxX = boxSize;
	float sizeBoxY = boxSize;
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

				position = glm::vec3((x * sizeBoxX) + (sizeBoxX / 2), box.getHeight() / 2, (y * sizeBoxY) + (sizeBoxY / 2));

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
	float sizeBoxX = boxSize;
	float sizeBoxY = boxSize;
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

void SceneController::bindAntTextures()
{
	glActiveTexture(GL_TEXTURE0);
	albedo.getTexture().bind(0);

	glActiveTexture(GL_TEXTURE1);
	normalMap.getTexture().bind(1);

	glActiveTexture(GL_TEXTURE2);
	metallicRoughnessMap.getTexture().bind(2);
}

void SceneController::unbindAntTextures()
{
	glActiveTexture(GL_TEXTURE0);
	albedo.getTexture().unbind(0);
	glActiveTexture(GL_TEXTURE1);
	normalMap.getTexture().unbind(1);
	glActiveTexture(GL_TEXTURE2);
	metallicRoughnessMap.getTexture().unbind(2);
}

