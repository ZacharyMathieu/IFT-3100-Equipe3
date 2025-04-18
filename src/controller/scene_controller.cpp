#include "scene_controller.h"

void SceneController::setup(int x, int y, int w, int h, GridController* gridController)
{
	ofDisableArbTex();
	checkPop = false;

	SCENE_X = x;
	SCENE_Y = y;
	SCENE_WIDTH = w;
	SCENE_HEIGHT = h;
	boxSize = 1;

	updateGridController(gridController);

	scale_ant = 0.002f * boxSize;

	box.set(gridController->scaleX * boxSize, boxSize * 5, gridController->scaleY * boxSize);
	box.mapTexCoords(0, 0, 2, 2);
	boxMesh = box.getMesh();

	pheromoneSquare.set(0, 0, gridController->GRID_WIDTH * boxSize, gridController->GRID_HEIGHT * boxSize);
	pheromoneMesh = pheromoneSquare.getMesh();
	
	antSphere.set(boxSize, 64);
	//vboBoxMeshAnt = antSphere.getMesh();

	tilePheromone.set(boxSize, boxSize);
	tilePheromone.setResolution(2, 2); 

	tilePheromoneMesh = tilePheromone.getMesh();

	cubeMap.load("images/sky.png", 2048, false);

	pheromoneSphere.set(boxSize, 12);
	foodSphere.set(boxSize, 12);
	foodSphereMesh = foodSphere.getMesh();
	//vboPheromone = pheromoneSphere.getMesh();
	slimes.load("models/slimes.obj");
	slimes.disableMaterials();

	for (int i = 0; i < slimes.getMeshCount(); i++) {
		slimesMesh.append(slimes.getMesh(i));
	}

	crackWall.load("images/crackWall.jpg");
	glitter.load("images/glitter.jpg");
	fire.load("images/fire.jpg");
	wood.load("images/wood.jpg");
	rock.load("images/rock.jpg");
	paint.load("images/paint.jpg");

	wallTextures.push_back(wood.getTexture());
	wallTextures.push_back(crackWall.getTexture());
	wallTextures.push_back(rock.getTexture());
	wallTextures.push_back(paint.getTexture());
	wallTextures.push_back(glitter.getTexture());
	wallTextures.push_back(fire.getTexture());

	texture.setTextureMinMagFilter(GL_LINEAR, GL_LINEAR);
	texture.setTextureWrap(GL_REPEAT, GL_REPEAT);

	//antModelLoader.loadModel("models/newAnt4/Ant_anim_fbx.fbx", true);
	antModelLoader.loadModel("models/sci-fiAnt/sci_fi_ant_unit.glb");

	antTexture.load("models/newAnt4/tex/Ant_color.jpg");
	antModelLoader.enableTextures();

	albedo.load("models/newAnt3/Textures/material_baseColor.jpg");
	normalMap.load("models/newAnt3/Textures/material_normal.png");
	metallicRoughnessMap.load("models/newAnt3/Textures/material_metallicRoughness.png");
	
	a = albedo.getTexture();
	n = normalMap.getTexture();
	m = metallicRoughnessMap.getTexture();
	a.setTextureMinMagFilter(GL_LINEAR, GL_LINEAR);
	n.setTextureMinMagFilter(GL_LINEAR, GL_LINEAR);
	m.setTextureMinMagFilter(GL_LINEAR, GL_LINEAR);

	ants.loadModel("models/newAnt4/Ant_anim_fbx.fbx", true);
	antMesh = ants.getMesh(0);
	vboAntMesh = antMesh;
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
	topCamera.setScale(0.1 * boxSize, -0.1 * boxSize, 0.1 * boxSize);
	topCamera.enableOrtho();
	topCamera.disableMouseInput();

	popUpCam = &topCamera;
	freeCamera.lookAt(ofVec3f(0, 0, 1));

	POV.setNearClip(0.1f);
	POV.disableMouseInput();
	cameras.push_back(&mainCamera);
	cameras.push_back(&topCamera);
	cameras.push_back(&freeCamera);
	cameras.push_back(&POV);
	numCam = 0;
	activeCam = cameras[numCam];
}

void SceneController::updateGridController(GridController* gridController)
{
	this->gridController = gridController;
	this->ant = gridController->ants[0];
}

ofxAssimpModelLoader& SceneController::getAntModel()
{
	return antModelLoader;
}

void SceneController::moveFreeCam()
{
	ofPoint lookAt = freeCamera.getLookAtDir();
	if (ofGetKeyPressed(OF_KEY_RIGHT) || ofGetKeyPressed('d')) freeCamPos -= (lookAt.getRotated(90, ofVec3f(0, 1, 0)) * ofVec3f(1, 0, 1)).getNormalized() * FREE_CAMERA_SPEED;
	if (ofGetKeyPressed(OF_KEY_LEFT) || ofGetKeyPressed('a')) freeCamPos += (lookAt.getRotated(90, ofVec3f(0, 1, 0)) * ofVec3f(1, 0, 1)).getNormalized() * FREE_CAMERA_SPEED;
	if (ofGetKeyPressed(OF_KEY_UP) || ofGetKeyPressed('w')) freeCamPos += (lookAt * ofVec3f(1, 0, 1)).getNormalized() * FREE_CAMERA_SPEED;
	if (ofGetKeyPressed(OF_KEY_DOWN) || ofGetKeyPressed('s')) freeCamPos -= (lookAt * ofVec3f(1, 0, 1)).getNormalized() * FREE_CAMERA_SPEED;
	if (ofGetKeyPressed(' ')) freeCamPos += ofVec3f(0, 1, 0) * FREE_CAMERA_SPEED;
	if (ofGetKeyPressed(OF_KEY_SHIFT)) freeCamPos -= ofVec3f(0, 1, 0) * FREE_CAMERA_SPEED;
}

ofPoint& SceneController::movePOV()
{
	ofPoint newPos = ant->pos;

	ofPoint lookAt = POV.getLookAtDir();
	ofPoint right = (lookAt.getRotated(90, ofVec3f(0, 1, 0)) * ofVec3f(1, 0, 1)).getNormalized();
	if (ofGetKeyPressed(OF_KEY_RIGHT) || ofGetKeyPressed('d'))
	{
		newPos.x -= right.x * (ANT_MOVE_SPEED * 2);
		newPos.y -= right.z * (ANT_MOVE_SPEED * 2);
	}
	if (ofGetKeyPressed(OF_KEY_LEFT) || ofGetKeyPressed('a'))
	{
		newPos.x += right.x * (ANT_MOVE_SPEED * 2);
		newPos.y += right.z * (ANT_MOVE_SPEED * 2);
	}
	if (ofGetKeyPressed(OF_KEY_UP) || ofGetKeyPressed('w'))
	{
		newPos.x += lookAt.x * (ANT_MOVE_SPEED * 2);
		newPos.y += lookAt.z * (ANT_MOVE_SPEED * 2);
	}
	if (ofGetKeyPressed(OF_KEY_DOWN) || ofGetKeyPressed('s'))
	{
		newPos.x -= lookAt.x * (ANT_MOVE_SPEED * 2);
		newPos.y -= lookAt.z * (ANT_MOVE_SPEED * 2);
	}

	return newPos;
}

void SceneController::move()
{
	ofVec3f newPos = ant->pos;
	float newAngle = ant->a;
	bool moved = false;

	if (activeCam == &freeCamera) {
		moveFreeCam();
	}
	else if (activeCam == &POV)
	{
		newPos = movePOV();
	}
	else
	{
		if (ofGetKeyPressed(OF_KEY_RIGHT) || ofGetKeyPressed('d'))
		{
			newPos.x = fmod((newPos.x + (ANT_MOVE_SPEED * 2)), gridController->GRID_WIDTH);
			newAngle = 0;
			moved = true;
		}
		if (ofGetKeyPressed(OF_KEY_LEFT) || ofGetKeyPressed('a'))
		{
			newPos.x = fmod((newPos.x - (ANT_MOVE_SPEED * 2) + (float)gridController->GRID_WIDTH), gridController->GRID_WIDTH);
			newAngle = PI;
			moved = true;
		}
		if (ofGetKeyPressed(OF_KEY_UP) || ofGetKeyPressed('w'))
		{
			newPos.y = fmod((newPos.y + (ANT_MOVE_SPEED * 2)), gridController->GRID_HEIGHT);
			newAngle = HALF_PI;
			moved = true;
		}
		if (ofGetKeyPressed(OF_KEY_DOWN) || ofGetKeyPressed('s'))
		{
			newPos.y = fmod((newPos.y - (ANT_MOVE_SPEED * 2) + gridController->GRID_HEIGHT), gridController->GRID_HEIGHT);
			newAngle = 3 * HALF_PI;
			moved = true;
		}
	}

	if (!checkCollision(newPos))
	{
		ant->a = newAngle;
		ant->pos = newPos;
	}
	else
	{
		moved = false;
	}

	if (animation || moved) {
		antModelLoader.getAnimation(0).stop();
		antModelLoader.getAnimation(1).play();
	}
	else
	{
		antModelLoader.getAnimation(1).stop();
		antModelLoader.getAnimation(0).play();
	}
}

void SceneController::update()
{
	antModelLoader.enableMaterials();
	centreX = 3 * (ofGetWidth() / 4.0f);
	centreY = ofGetHeight() / 2.0f;

	antModelLoader.setScale(scale_ant, -scale_ant, scale_ant);

	boxCollider.setPosition(antModelLoader.getPosition());

	move();

	antModelLoader.setRotation(0, ant->a * RAD_TO_DEG - 90, 0, 1, 0);
	antModelLoader.setRotation(1, -90, 1, 0, 0);

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

	POV.setPosition(antPos.x, antPos.y + 2, antPos.z);
	POV.lookAt(lookTarget);

	light.setPointLight();
	light.setDiffuseColor(ofColor(1, 1, 1));
	light.setSpecularColor(ofColor(191, 191, 191));

	light.setGlobalPosition(centreX, centreY, 255.0f);
	light.setOrientation(glm::vec3(antModelLoader.getPosition()));

	texture.setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
	texture.setTextureWrap(GL_REPEAT, GL_REPEAT);
	textureAnt.setTextureMinMagFilter(GL_LINEAR, GL_LINEAR);
	textureAnt.setTextureWrap(GL_REPEAT, GL_REPEAT);

	antModelLoader.update();
	if (animation) {
		antModelLoader.getAnimation(1).play();
	}
	else {
		antModelLoader.getAnimation(1).stop();
		antModelLoader.getAnimation(0).play();
	}
}

void SceneController::draw()
{

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

	freeCamera.setPosition(freeCamPos);
	activeCam->begin();
	drawScene();
	activeCam->end();
	freeCamera.setPosition(ofPoint());

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
	antModelLoader.drawFaces();

	shader_ant.begin();

	// Set uniform values
	shader_ant.setUniformTexture("texture0", wood.getTexture(), 0);
	shader_ant.setUniform3f("light_position", light.getGlobalPosition());
	shader_ant.setUniform3f("color_ambient", ant->COLOR.r / 255.0f, ant->COLOR.g / 255.0f, ant->COLOR.b / 255.0f);

	shader_ant.setUniform3f("color_diffuse", light.getDiffuseColor().r / 255.0f, light.getDiffuseColor().g / 255.0f, light.getDiffuseColor().b / 255.0f);

	shader_ant.setUniform3f("color_specular", light.getSpecularColor().r / 255.0f, light.getSpecularColor().g / 255.0f, light.getSpecularColor().b / 255.0f);
	shader_ant.setUniform1f("brightness", 10.0f);
	shader_ant.setUniform1f("uniform_scale", scale_ant / 3);

	// Matrices d'instances
	std::vector<glm::mat4> instanceMatrices;
	for (size_t i = 0; i < antPositions.size(); ++i) {
		if (!objectVisible(antPositions[i], RENDER_DISTANCE_ANTS)) continue;

		glm::mat4 translation = glm::translate(glm::mat4(1.0f), antPositions[i]);
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(antAngles[i]), glm::vec3(0, 1, 0));
		glm::mat4 modelMatrix = translation * rotation;
		instanceMatrices.push_back(modelMatrix);
	}

	if (!instanceMatrices.empty()) {
		// D�composer les colonnes
		std::vector<glm::vec4> col0, col1, col2, col3;
		for (const auto& m : instanceMatrices) {
			col0.push_back(m[0]);
			col1.push_back(m[1]);
			col2.push_back(m[2]);
			col3.push_back(m[3]);
		}

		// Envoyer chaque colonne comme attribut
		ofVbo& vbo = vboAntMesh.getVbo();
		vbo.setAttributeData(4, glm::value_ptr(col0[0]), 4, instanceMatrices.size(), GL_STATIC_DRAW);
		vbo.setAttributeData(5, glm::value_ptr(col1[0]), 4, instanceMatrices.size(), GL_STATIC_DRAW);
		vbo.setAttributeData(6, glm::value_ptr(col2[0]), 4, instanceMatrices.size(), GL_STATIC_DRAW);
		vbo.setAttributeData(7, glm::value_ptr(col3[0]), 4, instanceMatrices.size(), GL_STATIC_DRAW);

		vbo.setAttributeDivisor(4, 1);
		vbo.setAttributeDivisor(5, 1);
		vbo.setAttributeDivisor(6, 1);
		vbo.setAttributeDivisor(7, 1);

		// View / Projection
		shader_ant.setUniformMatrix4f("viewMatrix", activeCam->getModelViewMatrix());
		shader_ant.setUniformMatrix4f("projectionMatrix", activeCam->getProjectionMatrix());

		vboAntMesh.drawInstanced(OF_MESH_FILL, instanceMatrices.size());
	}

	shader_ant.end();

	shader.begin();
	ofColor* color = getFoodColor();
	shader.setUniform3f("color_ambient", color->r / 255.0, color->g / 255.0, color->b / 255.0);
	shader.setUniform3f("color_diffuse", 0, 1, 0);
	shader.setUniform3f("light_position", light.getGlobalPosition());

	for (auto& foodPos : foodPositions)
	{
		if (!objectVisible(foodPos, RENDER_DISTANCE_PHEROMONES))
			continue;

		shader.setUniform3f("translation", foodPos.x, foodPos.y, foodPos.z);
		shader.setUniform1f("scale_factor", 1);
		slimesMesh.drawFaces();
	}
	shader.end();

	//shader.begin();
	//shader.setUniform3f("color_ambient", 0, 0, 1);
	//shader.setUniform3f("color_diffuse", 0, 1, 0);
	//shader.setUniform3f("light_position", light.getGlobalPosition());
	//shader.setUniform3f("translation", 0, 0, 0);
	//shader.setUniform1f("scale_factor", 1);

	shader_texture_wall.begin();
	shader_texture_wall.setUniformTexture("texture0", texture, 0);
	shader_texture_wall.setUniform3f("color_ambient", 0.5f, 0.5f, 0.5f);
	shader_texture_wall.setUniform3f("color_diffuse", 1, 1, 1);
	shader_texture_wall.setUniform3f("light_position", light.getGlobalPosition());
	for (auto& pos : wallPositions)
	{
		if (objectVisible(pos, RENDER_DISTANCE_WALLS))
		{
			shader_texture_wall.setUniform3f("translation", pos.x, pos.y, pos.z);
			shader_texture_wall.setUniform1f("scale_factor", 1);
			float maxAniso;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
			texture.bind();
			boxMesh.draw(OF_MESH_FILL);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAniso);
			texture.unbind();
		}
	}

	shader_texture_wall.end();

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

	glm::vec3 pos;
	Cell* cell;

	ofPushMatrix();
	ofRotateDeg(-90, 1, 0, 0);
	for (auto& pheromone : pheromonePositions)
	{
		
		ofFill();
		pos = get<0>(pheromone);
		cell = get<1>(pheromone);
		if (!objectVisible(pos, RENDER_DISTANCE_PHEROMONES)) continue;

		ofPushMatrix();
		ofTranslate(pos.x + (boxSize/2), -pos.z- (boxSize/2), 1e-3 * boxSize);
		ofSetColor(cell->getCellColor(125));
		tilePheromoneMesh.draw();
		ofPopMatrix();
	}

	ofTranslate(0, 0, -1e-3 * boxSize);
	ofSetColor(255, 255, 255, 125);
	ofDrawRectangle(0, 0, gridController->GRID_WIDTH * boxSize, -gridController->GRID_HEIGHT * boxSize);
	ofPopMatrix();
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
	return (glm::dot(glm::normalize(activeCam->getLookAtDir()), glm::normalize(pos - activeCam->getPosition())) < 0) || (glm::distance(activeCam->getPosition(), pos) < renderDistance);
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
	foodPositions.clear();

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
			else if (cell->type == FOOD)
			{
				position = glm::vec3(((x * sizeBoxX) + (sizeBoxX / 2)), 0, (y * sizeBoxY) + (sizeBoxY / 2));

				foodPositions.push_back(position);
			}
			else if (cell->type == PHEROMONE && cell->value > 0)
			{

				position = glm::vec3(x * sizeBoxX, 0, y * sizeBoxY);

				//pheromoneColorCache[position] = ofRandom(0, cell->getValueFactor());
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
			position = glm::vec3((posAnt.x * sizeBoxX) + (sizeBoxX / 2), 0.5, (posAnt.y * sizeBoxY) + (sizeBoxY / 2));

			antPositions.push_back(position);
			antAngles.push_back(ant->a * RAD_TO_DEG + 180);
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

