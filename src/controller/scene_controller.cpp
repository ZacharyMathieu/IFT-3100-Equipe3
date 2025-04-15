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

	antSphere.set(boxSize, 64);
	//vboBoxMeshAnt = antSphere.getMesh();

	cubeMap.load("images/sky.png", 2048, false);

	pheromoneSphere.set(boxSize, 12);
	vboPheromone = pheromoneSphere.getMesh();
	slimes.load("models/slimes.obj");
	slimes.disableMaterials();
	for (int i = 0; i < slimes.getNumMeshes(); ++i) {
		ofMesh mesh = slimes.getMesh(i);
		ofMatrix4x4 mat = slimes.getMeshHelper(i).matrix;

		// Convertir ofMatrix4x4 en glm::mat4
		glm::mat4 glmMatrix = glm::make_mat4(mat.getPtr());

		// Extraire la rotation seulement pour les normales (mat3)
		glm::mat3 glmRot = glm::mat3(glmMatrix);  // coupe les colonnes de translation

		// Appliquer transformation aux vertices
		vector<glm::vec3> vertices = mesh.getVertices();
		for (auto& v : vertices) {
			glm::vec4 transformed = glmMatrix * glm::vec4(v, 1.0f);
			v = glm::vec3(transformed);
		}
		mesh.clearVertices();
		mesh.addVertices(vertices);

		// Appliquer rotation aux normales
		vector<glm::vec3> normals = mesh.getNormals();
		for (auto& n : normals) {
			n = glm::normalize(glmRot * n);
		}
		mesh.clearNormals();
		mesh.addNormals(normals);

		// Ajouter au mesh final
		slimesMesh.append(mesh);
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
	antModelLoader.loadModel("models/sci-fiAnt/ant-SciFi.gltf");

	antTexture.load("models/newAnt4/tex/Ant_color.jpg");
	antModelLoader.enableTextures();

	albedo.load("models/sci-fiAnt/texture/Image_0.png");
	normalMap.load("models/sci-fiAnt/texture/Image_3.png");
	metallic.load("models/sci-fiAnt/texture/Image_2.png");
	roughness.load("models/sci-fiAnt/texture/Image_1.png");


	ants.loadModel("models/sci-fiAnt/ant-SciFi.gltf", true);
	ofMesh antMesh;

	for (int i = 0; i < ants.getNumMeshes(); ++i) {
		ofMesh mesh = ants.getMesh(i);
		ofMatrix4x4 mat = ants.getMeshHelper(i).matrix;

		// Convertir ofMatrix4x4 en glm::mat4
		glm::mat4 glmMatrix = glm::make_mat4(mat.getPtr());

		// Extraire la rotation seulement pour les normales (mat3)
		glm::mat3 glmRot = glm::mat3(glmMatrix);  // coupe les colonnes de translation

		// Appliquer transformation aux vertices
		vector<glm::vec3> vertices = mesh.getVertices();
		for (auto& v : vertices) {
			glm::vec4 transformed = glmMatrix * glm::vec4(v, 1.0f);
			v = glm::vec3(transformed);
		}
		mesh.clearVertices();
		mesh.addVertices(vertices);

		// Appliquer rotation aux normales
		vector<glm::vec3> normals = mesh.getNormals();
		for (auto& n : normals) {
			n = glm::normalize(glmRot * n);
		}
		mesh.clearNormals();
		mesh.addNormals(normals);

		// Ajouter au mesh final
		antMesh.append(mesh);
	}

	vboAntMesh = antMesh;
	ofLog() << vboAntMesh.getTexCoords()[0];
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
	freeCamera.lookAt(ofVec3f(0, 0, 0));

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

void SceneController::move()
{
	//merci ChatGPT pour quelques conseils
	ofPoint lookAt = POV.getLookAtDir();

	ofVec3f newPos = ant->pos;
	float newAngle = ant->a;
	glm::vec3 lookDir = glm::normalize(POV.getLookAtDir());
	glm::vec3 forward = glm::normalize(glm::vec3(lookDir.x, 0, lookDir.z));
	glm::vec3 right = glm::normalize(glm::vec3(forward.z, 0, -forward.x));

	if (activeCam == &freeCamera) {
		moveFreeCam();
	}
	else
	{
		bool moved = false;
		if (ofGetKeyPressed(OF_KEY_RIGHT) || ofGetKeyPressed('d'))
		{
			if (activeCam == &POV) {
				newPos.x -= right.x * (ANT_MOVE_SPEED * 2);
				newPos.y -= right.z * (ANT_MOVE_SPEED * 2);
			}
			else
			{
				newPos.x = fmod((newPos.x + (ANT_MOVE_SPEED * 2)), gridController->GRID_WIDTH);
				newAngle = 0;
			}
			moved = true;
		}
		if (ofGetKeyPressed(OF_KEY_LEFT) || ofGetKeyPressed('a'))
		{
			if (activeCam == &POV) {
				newPos.x += right.x * (ANT_MOVE_SPEED * 2);
				newPos.y += right.z * (ANT_MOVE_SPEED * 2);

			}
			else
			{
				newPos.x = fmod((newPos.x - (ANT_MOVE_SPEED * 2) + (float)gridController->GRID_WIDTH), gridController->GRID_WIDTH);
				newAngle = PI;
			}
			moved = true;
		}
		if (ofGetKeyPressed(OF_KEY_UP) || ofGetKeyPressed('w'))
		{
			if (activeCam == &POV) {
				newPos.x += forward.x * (ANT_MOVE_SPEED * 2);
				newPos.y += forward.z * (ANT_MOVE_SPEED * 2);

			}
			else {
				newPos.y = fmod((newPos.y + (ANT_MOVE_SPEED * 2)), gridController->GRID_HEIGHT);
				newAngle = HALF_PI;
			}
			moved = true;
		}
		if (ofGetKeyPressed(OF_KEY_DOWN) || ofGetKeyPressed('s'))
		{
			if (activeCam == &POV) {
				newPos.x -= forward.x * (ANT_MOVE_SPEED * 2);
				newPos.y -= forward.z * (ANT_MOVE_SPEED * 2);
			}
			else
			{
				newPos.y = fmod((newPos.y - (ANT_MOVE_SPEED * 2) + gridController->GRID_HEIGHT), gridController->GRID_HEIGHT);
				newAngle = 3 * HALF_PI;
			}
			moved = true;
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
	shader_ant.setUniformTexture("albedoMap", albedo.getTexture(), 0);
	shader_ant.setUniformTexture("normalMap", normalMap.getTexture(), 1);
	shader_ant.setUniformTexture("metallicMap", metallic.getTexture(), 2);
	shader_ant.setUniformTexture("roughnessMap", roughness.getTexture(), 3);
	shader_ant.setUniform3f("light_position", light.getGlobalPosition());

	shader_ant.setUniform3f("view_position", activeCam->getGlobalPosition());
	shader_ant.setUniform1f("uniform_scale", scale_ant/8);

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

		vboAntMesh.drawInstanced(OF_MESH_FILL, instanceMatrices.size());
	}

	shader_ant.end();

	bool visible;
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

		if (!objectVisible(pos, RENDER_DISTANCE_PHEROMONES))
			continue;

		int posX = playMode ? pos.x + ofRandom(-1, 1) : pos.x;
		int posZ = playMode ? pos.z + ofRandom(-1, 1) : pos.z;
		int scalePheromone = playMode ? cell->getValueFactor() * ofRandom(2, 5) : cell->getValueFactor() * 2;

		shader.setUniform3f("translation", posX, pos.y, posZ);
		shader.setUniform1f("scale_factor", scalePheromone);
		slimesMesh.draw();

		//*****D�commenter si on veut ajouter des pheromones autour.***

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

	ofFill();
	ofSetLineWidth(10);
	ofSetColor(250);

	ofPushMatrix();
	ofTranslate(0, 0, 0);
	ofRotateDeg(-90, 1, 0, 0);
	ofSetColor(255, 255, 255, 125);
	ofDrawRectangle(0, 0, gridController->GRID_WIDTH * boxSize, -gridController->GRID_HEIGHT * boxSize);
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
			position = glm::vec3((posAnt.x * sizeBoxX) + (sizeBoxX / 2), 0, (posAnt.y * sizeBoxY) + (sizeBoxY / 2));

			antPositions.push_back(position);
			antAngles.push_back(ant->a * RAD_TO_DEG + 180);
		}
	}
}



