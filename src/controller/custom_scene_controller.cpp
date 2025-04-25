#include "custom_scene_controller.h"

glm::vec3 CustomSceneController::catmullRom(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, float t)
{
	return 0.5f * (
		2.0f * p1 +
		(-p0 + p2) * t +
		(2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * t * t +
		(-p0 + 3.0f * p1 - 3.0f * p2 + p3) * t * t * t
		);
}
glm::vec3 CustomSceneController::bezier(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, float t)
{
	float u = 1.0f - t;
	float tt = t * t;
	float uu = u * u;
	float uuu = uu * u;
	float ttt = tt * t;

	glm::vec3 point = uuu * p0; // (1 - t)^3 * p0
	point += 3 * uu * t * p1;   // 3 * (1 - t)^2 * t * p1
	point += 3 * u * tt * p2;   // 3 * (1 - t) * t^2 * p2
	point += ttt * p3;          // t^3 * p3
	return point;
}
glm::vec3 CustomSceneController::bezier6(const glm::vec3& p0,
	const glm::vec3& p1,
	const glm::vec3& p2,
	const glm::vec3& p3,
	const glm::vec3& p4,
	const glm::vec3& p5,
	float t)
{
	float u = 1.0f - t;
	float tt = t * t;
	float ttt = tt * t;
	float tttt = ttt * t;
	float ttttt = tttt * t;

	float uu = u * u;
	float uuu = uu * u;
	float uuuu = uuu * u;
	float uuuuu = uuuu * u;

	glm::vec3 point = uuuuu * p0;
	point += 5 * uuuu * t * p1;
	point += 10 * uuu * tt * p2;
	point += 10 * uu * ttt * p3;
	point += 5 * u * tttt * p4;
	point += ttttt * p5;

	return point;
}



void CustomSceneController::deformTablette()
{
	if (controlPoints.size() != 6) return;

	tabletteMesh = tabletteMeshOriginal;  // Reset le mesh

	for (int i = 0; i < tabletteMesh.getNumVertices(); ++i) {
		glm::vec3 v = tabletteMeshOriginal.getVertex(i);

		// Mapping X de -25 � +25 (comme largeur tablette)
		float tNorm = ofMap(v.x, -25, 25, 0.0f, 1.0f, true);
		glm::vec3 curved = bezier6(controlPoints[0], controlPoints[1], controlPoints[2],
			controlPoints[3], controlPoints[4], controlPoints[5], tNorm);

		// Affecte la courbure � l'axe Y
		v.y += curved.y;
		tabletteMesh.setVertex(i, v);
	}

}

void CustomSceneController::setup()
{
	auto glfwWindow = dynamic_pointer_cast<ofAppGLFWWindow>(ofGetMainLoop()->getCurrentWindow());
	GLFWwindow* mainWindow = glfwWindow->getGLFWWindow();

	glfwSetCursor(mainWindow, glfwCreateStandardCursor(GLFW_ARROW_CURSOR));

	ofDisableArbTex(); // dans setup() AVANT le chargement des images

	ofEnableLighting();
	ofSetFrameRate(60);
	ofSetVerticalSync(true);

	mirrorFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);

	leftPos = rightTarget = glm::vec3(49, 0, 0);
	rightPos = leftTarget = glm::vec3(-49, 0, 0);
	posterPos = glm::vec3(0, 5, 25);
	posterTarget = glm::vec3(0, 5, -25);

	isMaterial = false;

	cadre.load("images/cadre.png");
	ck = ConvolutionKernel::identity;

	// Load ton mod�le
	ant.loadModel("models/sci-fiAnt/ant-SciFi.gltf");
	redAnt.loadModel("models/sci-fiAnt2/ant-SciFi.gltf");
	greenAnt.loadModel("models/sci-fiAnt3/ant-SciFi.gltf");
	antWithMaterial.loadModel("models/sci-fiAnt/sci_fi_ant_unit.glb");

	ants.push_back(&ant);
	ants.push_back(&redAnt);
	ants.push_back(&greenAnt);

	reloadShaders();

	antTexture = ant.getTextureForMesh(0);
	ofImage baseColorImg;
	baseColorImg.load("models/sci-fiAnt/texture/Image_0.png");  // Remplace par le vrai chemin
	baseColorTexture = baseColorImg.getTexture();

	ofImage normalImg;
	normalImg.load("models/sci-fiAnt/texture/Image_3.png");  // Remplace par le vrai chemin
	normalMapTexture = normalImg.getTexture();

	blueEye.load("models/sci-fiAnt/texture/Image_2.png");  // Remplace par le vrai chemin
	metallicTexture = blueEye.getTexture();

	redEye.load("models/sci-fiAnt/texture/Image_2_red.png");
	greenEye.load("models/sci-fiAnt/texture/Image_2_green.png");
	doubleColorEye.load("models/sci-fiAnt/texture/Image_2_gp.png");

	ofImage roughnessImg;
	roughnessImg.load("models/sci-fiAnt/texture/Image_1.png");  // Remplace par le vrai chemin
	roughnessTexture = roughnessImg.getTexture();

	imgPlateform.load("images/wood.jpg");
	wood.load("images/wood.jpg");
	brick.load("images/rock.jpg");

	texPlateform = imgPlateform.getTexture();

	activeAnt = ants[0];
	activeAnt->setPosition(0, 0, 0); // local position
	antTransform.setPosition(0, -45, 0);
	antTransform.setScale(antScale, -antScale, antScale);
	activeAnt->getAnimation(0).play();
	antWithMaterial.getAnimation(0).play();
	
	// Cr�e les plans
	float boxSize = 100;
	float wallThickness = 1.0;

	// Sol
	floor.set(boxSize + 20, boxSize + 20);
	floor.rotateDeg(90, 1, 0, 0);
	floor.setPosition(0, -50, 0);

	// Plafond
	ceiling.set(boxSize + 20, boxSize + 20);
	ceiling.rotateDeg(90, 1, 0, 0);
	ceiling.setPosition(0, 50, 0);

	// Mur arri�re
	backWall.set(boxSize, boxSize);
	backWall.setPosition(0, 0, -boxSize / 2);

	ofPoint cadrePos = ofPoint(0, -10, backWall.getPosition().z + 1e-3);
	cadrePlane.set(90, 70);
	cadrePlane.setPosition(cadrePos.x, cadrePos.y, cadrePos.z);
	cadrePlane.rotateDeg(180, 0, 1, 0);

	poster.set(56, 48);
	poster.setPosition(cadrePos.x, cadrePos.y - 1, cadrePos.z + 1e-3);
	//poster.setPosition(1, -1, backWall.getPosition().z + 0.2);
	poster.rotateDeg(180, 0, 1, 0);

	image_height = posterImg.getHeight();
	image_width = posterImg.getWidth();

	posterFilter.allocate(image_width, image_height, OF_IMAGE_COLOR);

	// Mur gauche
	leftWall.set(boxSize + 20, boxSize);
	leftWall.rotateDeg(90, 0, 1, 0);
	leftWall.setPosition(-boxSize / 2, 0, 0);
	planeMeshLeft = leftWall.getMesh();

	// Mur droit
	rightWall.set(boxSize + 20, boxSize + 5, 2);
	rightWall.setResolution(100, 100, 1);
	rightWall.rotateDeg(90, 0, 1, 0);
	rightWall.setPosition(boxSize / 2, 0, 0);
	planeMeshRight = rightWall.getMesh();

	plateform.set(40, 5);
	plateform.setResolution(64, 1);
	plateform.setPosition(0, -48, 0);

	//tablette
	tablette.set(50, 2, 10, 50, 10, 1); // (width, height, depth, resX, resY, resZ)
	tablette2.set(50, 2, 10, 50, 10, 1);
	tablette.setPosition((boxSize / 2) - 5, -20, 0);
	tablette2.setPosition(-(boxSize / 2) + 5, -20, 0);
	tablette2Mesh = tablette2.getMesh();
	tabletteMeshOriginal = tablette.getMesh();

	vase.load("models/vase.glb", true);
	vase.setPosition(tablette2.getPosition().x, tablette2.getPosition().y, tablette2.getPosition().z);
	vase.setScale(0.02, 0.02, 0.02);
	vase.setRotation(0, -90, 1, 0, 0);

	controlPoints = {
	glm::vec3(-25, 0, 0),    // d�part
	glm::vec3(-15, 20, 0),   // vers le haut
	glm::vec3(-5, -10, 0),   // vers le bas
	glm::vec3(5, 10, 0),     // vers le haut
	glm::vec3(15, -10, 0),   // vers le bas
	glm::vec3(25, 0, 0)      // fin
	};

	// Cam�ra
	firstPos = glm::vec3(0, -10, 100);
	firstTarget = glm::vec3(0, -10, 0);
	cam.setPosition(firstPos);
	cam.lookAt(firstTarget);

	//GUI avec materiaux
	gui.setup("With Materials");
	gui.setPosition(10, 10);

	useMaterial.setName("Use material");
	gui.add(useMaterial);
	useMaterial.addListener(this, &CustomSceneController::onUseMaterial);
	useMaterial = true;
	blueTint.setName("Blue eyes");
	gui.add(blueTint);
	blueTint.addListener(this, &CustomSceneController::onBlueChanged);
	redTint.setName("Red eyes");
	gui.add(redTint);
	redTint.addListener(this, &CustomSceneController::onRedChanged);
	greenTint.setName("Green eyes");
	gui.add(greenTint);
	greenTint.addListener(this, &CustomSceneController::onGreenChanged);

	//GUI sans materiaux
	tintGui.setup("Without Materials");
	tintGui.setPosition(gui.getWidth() + 10, 10);
	noMaterial.setName("No material");
	tintGui.add(noMaterial);
	noMaterial.addListener(this, &CustomSceneController::onNoMaterial);
	noMaterial = false;
	tintGui.add(upperColor.set("Upper part", ofColor(255, 255, 255), ofColor(0, 0, 0), ofColor(255, 255, 255)));
	tintGui.add(bottomColor.set("Bottom part", ofColor(0, 140, 255), ofColor(0, 0, 0), ofColor(255, 255, 255)));


	//GUI mur droit
	guiRight.setup("Texture wall");
	guiRight.setPosition(gui.getPosition());
	colorChoiceRight.setName("Color wall");
	guiRight.add(colorChoiceRight);
	colorChoiceRight.addListener(this, &CustomSceneController::onColorRightPick);
	colorChoiceRight = true;
	guiRight.add(colorPickerRight.set("color piker", ofColor(0, 255, 0), ofColor(0, 0, 0), ofColor(255, 255, 255)));
	posterChoiceRight.setName("Texture choice");
	guiRight.add(posterChoiceRight);
	posterChoiceRight.addListener(this, &CustomSceneController::onTexturePick);
	posterChoiceRight = false;
	reliefActivatedRight.setName("relief activated");
	guiRight.add(reliefActivatedRight);
	reliefActivatedRight.addListener(this, &CustomSceneController::onReliefSelected);
	reliefActivatedRight = false;
	woodPick.setName("Wood texture");
	guiRight.add(woodPick);
	woodPick.addListener(this, &CustomSceneController::onWoodPick);
	woodPick = true;
	brickPick.setName("Rock texture");
	guiRight.add(brickPick);
	brickPick.addListener(this, &CustomSceneController::onBrickPick);
	brickPick = false;


	//GUI mur gauche
	guiLeft.setup("Poster wall");
	guiLeft.setPosition(gui.getPosition());
	colorChoiceLeft.setName("Color wall");
	colorChoiceLeft.addListener(this, &CustomSceneController::onColorLeftPick);
	guiLeft.add(colorChoiceLeft);
	colorChoiceLeft = true;
	guiLeft.add(colorPickerLeft.set("color piker", ofColor(255, 0, 0), ofColor(0, 0, 0), ofColor(255, 255, 255)));
	posterPick.setName("Poster wall");
	guiLeft.add(posterPick);
	posterPick.addListener(this, &CustomSceneController::onPosterSet);
	posterPick = false;
	posterChoiceLeft.setName("Choice your poster");
	guiLeft.add(posterChoiceLeft);
	posterChoiceLeft = false;
	identite.setName("Identity filter");
	identite.addListener(this, &CustomSceneController::onIdentityChanged);
	guiLeft.add(identite);
	aiguiser.setName("Sharpen filter");
	aiguiser.addListener(this, &CustomSceneController::onSharpChanged);
	guiLeft.add(aiguiser);
	border.setName("Edge detect filter");
	border.addListener(this, &CustomSceneController::onBorderChanged);
	guiLeft.add(border);
	bosseler.setName("Emboss filter");
	bosseler.addListener(this, &CustomSceneController::onEmbossChanged);
	guiLeft.add(bosseler);
	flou.setName("blur filter");
	flou.addListener(this, &CustomSceneController::onBlurChanged);
	guiLeft.add(flou);

	identite = aiguiser = border = bosseler = flou = false;

	blueTint = true;
	redTint = false;
	greenTint = false;
	doubleTint = false;

	filterActivated = false;


	//Gui B�zier
	controlPointsGui.setup("Deformation Tablette");
	controlPointsGui.setPosition(10, 400);

	for (int i = 0; i < controlPoints.size(); ++i) {
		ofParameter<float> slider;
		slider.set("Pt " + ofToString(i + 1), controlPoints[i].y, -30, 50);

		controlPointYSliders.push_back(slider);
		controlPointsGui.add(controlPointYSliders.back());

		controlPointYSliders.back().addListener(this, &CustomSceneController::onControlPointYChanged);
		controlPointYSliders[i] = 0.0f;
	}

	resetButton.set(ofGetWidth() / 2 - 50, 5, 75, 40);
	cam.disableMouseInput();

	lightPanel.setup("Light");
	lightPanel.setPosition(ofGetWidth() - lightPanel.getWidth() - 10, 10);

	material_brightness.set("material_brightness", 1, 0, 1);
	material_metallic.set("material_metallic", 0.1, 0, 1);
	material_roughness.set("material_roughness", 0.2, 0, 1);
	material_occlusion.set("material_occlusion", 0.6, 0, 1);
	material_fresnel_ior.set("material_fresnel_ior", ofPoint(0.5, 0.5, 0.5));
	tone_mapping_exposure.set("tone_mapping_exposure", 1, 0, 1);
	tone_mapping_toggle.set("tone_mapping_toggle", true);
	tone_mapping_gamma.set("tone_mapping_gamma", 1, 0, 1);
	light_position.set("light_position", ofPoint(0), ofPoint(-boxSize / 2, -boxSize / 2, -boxSize / 2), ofPoint(boxSize / 2, boxSize / 2, boxSize / 2));
	light_color.set("light_color", ofColor(255, 150, 0));
	light_intensity.set("light_intensity", 0.1, 0, 1);
	material_color_ambient.set("material_color_ambient", ofColor(255));
	material_color_diffuse.set("material_color_diffuse", ofColor(255));
	material_color_specular.set("material_color_specular", ofColor(255));

	lightPanel.add(material_brightness);
	lightPanel.add(material_metallic);
	lightPanel.add(material_roughness);
	lightPanel.add(material_occlusion);
	lightPanel.add(material_fresnel_ior);
	lightPanel.add(tone_mapping_exposure);
	lightPanel.add(tone_mapping_toggle);
	lightPanel.add(tone_mapping_gamma);
	lightPanel.add(light_position);
	lightPanel.add(light_color);
	lightPanel.add(light_intensity);
	lightPanel.add(material_color_ambient);
	lightPanel.add(material_color_diffuse);
	lightPanel.add(material_color_specular);

	texture_metallic = ofImage("texture/metal_plate_metallic_1k.jpg").getTexture();
	texture_roughness = ofImage("texture/metal_plate_roughness_1k.jpg").getTexture();
	texture_occlusion = ofImage("texture/metal_plate_ao_1k.jpg").getTexture();

	lightBall.setRadius(1);
}

void CustomSceneController::reloadShaders()
{
	shader.load("custom_ant_330_vs.glsl", "custom_ant_330_fs.glsl");
	//lightShader.load("light_330_vs.glsl", "light_330_fs.glsl");
	lightTextureShader.load("light_330_vs.glsl", "light_330_fs.glsl");
	//lightTextureShader.load("light_texture_330_vs.glsl", "light_texture_330_fs.glsl");
}

void CustomSceneController::update()
{
	gui.loadFont("verdana.ttf", 12);
	guiLeft.loadFont("verdana.ttf", 12);
	guiRight.loadFont("verdana.ttf", 12);
	tintGui.loadFont("verdana.ttf", 12);
	lightPanel.loadFont("verdana.ttf", 12);

	auto camPos = cam.getPosition();
	float mirrorZ = 2 * poster.getPosition().z - camPos.z;
	float posterWidth = poster.getWidth();
	float posterHeight = poster.getHeight();
	mirrorCam.setPosition(camPos.x, camPos.y, mirrorZ);
	//mirrorCam.lookAt(cam.getPosition());
	mirrorCam.setupOffAxisViewPortal(
		poster.getPosition() + glm::vec3(posterWidth, posterHeight, 0) / 2,
		poster.getPosition() + glm::vec3(posterWidth, -posterHeight, 0) / 2,
		poster.getPosition() + glm::vec3(-posterWidth, -posterHeight, 0) / 2
	);

	if (ofGetKeyPressed(OF_KEY_RIGHT))
	{
		newAngle += turnSpeed;
	}

	if (ofGetKeyPressed(OF_KEY_LEFT))
	{
		newAngle -= turnSpeed;
	}

	lightBall.setPosition(light_position.get().x, light_position.get().y, light_position.get().z);

	antTransform.setOrientation(glm::vec3(0, newAngle, 0));

	plateform.setOrientation(glm::vec3(0, newAngle, 0));
	activeAnt->update();
	activeAnt->getAnimation(0).play();
	antWithMaterial.getAnimation(0).play();
	antWithMaterial.setScale(0.50, 0.50, 0.50);
	antWithMaterial.setRotation(0, -90, 1, 0, 0);
	antWithMaterial.setRotation(1, newAngle, 0, 0, 1);

	if (isTransitioning) {
		float elapsed = ofGetElapsedTimef() - transitionStartTime;
		float t = ofClamp(elapsed / transitionDuration, 0.0f, 1.0f);

		glm::vec3 currentPos = glm::mix(startPos, endPos, t);
		glm::vec3 currentTarget = glm::mix(startTarget, endTarget, t);

		cam.setPosition(currentPos);
		cam.lookAt(currentTarget);

		if (t >= 1.0f) {
			isTransitioning = false;
		}
	}
	if (identite || aiguiser || bosseler || border || flou) {

	}
	else {
		filterActivated = false;
	}
	upperColorChoice = upperColor;
	bottomColorChoice = bottomColor;
	rightWallColor = colorPickerRight;
	leftWallColor = colorPickerLeft;

	if (posterChoiceLeft) openPosterChoicer();
	deformTablette();
}

void CustomSceneController::draw()
{
	mirrorFbo.begin();
	ofClear(0);
	ofBackground(0);
	mirrorCam.begin();
	drawScene(false, mirrorCam.getModelViewMatrix());
	mirrorCam.end();
	mirrorFbo.end();

	ofClear(0);
	ofBackground(0);
	cam.begin();
	drawScene(true, cam.getModelViewMatrix());
	cam.end();

	if (cam.getPosition() == leftPos || cam.getPosition() == rightPos || cam.getPosition() == posterPos) {
		ofFill();
		ofSetColor(0); // gris clair
		ofDrawRectangle(resetButton);
		ofNoFill();
		ofSetColor(100);
		ofDrawRectangle(resetButton);
		ofSetColor(255); // texte noir
		ofDrawBitmapString("Go Back", resetButton.x + 10, resetButton.y + 25);
	}

	drawGUI();
}

void CustomSceneController::drawScene(bool withMirror, glm::mat4& modelViewMatrix)
{
	ofEnableDepthTest();
	ofSetColor(light_color);
	lightBall.draw();

	lightTextureShader.begin();
	lightTextureShader.setUniform1f("material_brightness", material_brightness);
	lightTextureShader.setUniform1f("material_metallic", material_metallic);
	lightTextureShader.setUniform1f("material_roughness", material_roughness);
	lightTextureShader.setUniform1f("material_occlusion", material_occlusion);
	lightTextureShader.setUniform3f("material_fresnel_ior", glm::vec3(material_fresnel_ior.get().x, material_fresnel_ior.get().y, material_fresnel_ior.get().z));
	lightTextureShader.setUniform1f("tone_mapping_exposure", tone_mapping_exposure);
	lightTextureShader.setUniform1f("tone_mapping_toggle", tone_mapping_toggle);
	lightTextureShader.setUniform1f("tone_mapping_gamma", tone_mapping_gamma);
	lightTextureShader.setUniform3f("light_position", modelViewMatrix * glm::vec4(light_position.get().x, light_position.get().y, light_position.get().z, 1));
	lightTextureShader.setUniform3f("light_color", glm::vec3(light_color.get().r / 255.0, light_color.get().g / 255.0, light_color.get().b / 255.0));
	lightTextureShader.setUniform1f("light_intensity", light_intensity);
	lightTextureShader.setUniform3f("material_color_ambient", glm::vec3(material_color_ambient.get().r / 255.0, material_color_ambient.get().g / 255.0, material_color_ambient.get().b / 255.0));
	lightTextureShader.setUniform3f("material_color_diffuse", glm::vec3(material_color_diffuse.get().r / 255.0, material_color_diffuse.get().g / 255.0, material_color_diffuse.get().b / 255.0));
	lightTextureShader.setUniform3f("material_color_specular", glm::vec3(material_color_specular.get().r / 255.0, material_color_specular.get().g / 255.0, material_color_specular.get().b / 255.0));

	lightTextureShader.setUniformTexture("texture_metallic", texture_metallic, 0);
	lightTextureShader.setUniformTexture("texture_roughness", texture_roughness, 0);
	lightTextureShader.setUniformTexture("texture_occlusion", texture_occlusion, 0);

	ofPixels colorPixels;
	ofTexture colorTexture;
	colorPixels.allocate(1, 1, OF_PIXELS_RGBA);
	colorTexture.allocate(colorPixels);
	colorPixels.setColor(0, 0, ofColor(255));
	colorTexture.loadData(colorPixels);

	if (withMirror)
	{
		lightTextureShader.setUniform1f("material_brightness", 1);
		lightTextureShader.setUniform1f("material_metallic", 0);
		lightTextureShader.setUniform1f("material_roughness", 0);
		lightTextureShader.setUniform1f("material_occlusion", 1);
		lightTextureShader.setUniformTexture("texture_diffuse", mirrorFbo.getTexture(), 0);
		poster.draw();

		lightTextureShader.setUniform1f("material_brightness", material_brightness);
		lightTextureShader.setUniform1f("material_metallic", material_metallic);
		lightTextureShader.setUniform1f("material_roughness", material_roughness);
		lightTextureShader.setUniform1f("material_occlusion", material_occlusion);
		lightTextureShader.setUniformTexture("texture_diffuse", cadre.getTexture(), 0);
		cadrePlane.draw();

		colorPixels.setColor(0, 0, ofColor(200)); // Mur arri�re gris
		colorTexture.loadData(colorPixels);
		lightTextureShader.setUniformTexture("texture_diffuse", colorTexture, 0);
		backWall.draw();
	}

	// Dessine les murs (avec couleurs distinctes)
	colorPixels.setColor(0, 0, ofColor(180));  // Sol gris clair
	colorTexture.loadData(colorPixels);
	lightTextureShader.setUniformTexture("texture_diffuse", colorTexture, 0);
	floor.draw();

	colorPixels.setColor(0, 0, ofColor(255)); // Plafond blanc
	colorTexture.loadData(colorPixels);
	lightTextureShader.setUniformTexture("texture_diffuse", colorTexture, 0);
	ceiling.draw();

	ofPushMatrix();
	ofTranslate(leftWall.getPosition());
	ofRotateDeg(90, 0, 1, 0);
	if (!posterSet) {
		colorPixels.setColor(0, 0, leftWallColor);
		colorTexture.loadData(colorPixels);
		lightTextureShader.setUniformTexture("texture_diffuse", colorTexture, 0);
	}
	else
	{
		if (filterActivated)
		{
			lightTextureShader.setUniformTexture("texture_diffuse", posterFilter, 0);
		}
		else {
			lightTextureShader.setUniformTexture("texture_diffuse", posterImg, 0);
		}
	}
	planeMeshLeft.draw();
	ofPopMatrix();

	ofPushMatrix();
	ofTranslate(rightWall.getPosition());
	ofRotateDeg(90, 0, 1, 0);
	if (!textureActivated) {
		colorPixels.setColor(0, 0, rightWallColor);
		colorTexture.loadData(colorPixels);
		lightTextureShader.setUniformTexture("texture_diffuse", colorTexture, 0);
	}
	else
	{
		lightTextureShader.setUniformTexture("texture_diffuse", wallTexture, 0);
	}
	planeMeshRight.draw();
	ofPopMatrix();

	ofPushMatrix();
	lightTextureShader.setUniformTexture("texture_diffuse", texPlateform, 0);
	plateform.draw();
	ofPopMatrix();

	ofPushMatrix();
	ofTranslate(tablette.getPosition());
	ofRotateDeg(90, 0, 1, 0);  // Orienter comme le mur droit
	ofSetColor(255);
	lightTextureShader.setUniformTexture("texture_diffuse", texPlateform, 0);
	tabletteMesh.draw();
	ofPopMatrix();

	ofPushMatrix();
	ofTranslate(tablette2.getPosition());
	ofRotateDeg(90, 0, 1, 0);  // Orienter comme le mur droit
	ofSetColor(200);
	lightTextureShader.setUniformTexture("texture_diffuse", texPlateform, 0);
	tablette2Mesh.draw();
	ofPopMatrix();

	lightTextureShader.setUniformTexture("texture_diffuse", vase.getTextureForMesh(0), 0);
	ofPushMatrix();
	ofMultMatrix(vase.getModelMatrix());
	vase.getMesh(0).draw();
	ofPopMatrix();

	
	ofPushMatrix();
	ofMultMatrix(antTransform.getGlobalTransformMatrix());
	ofMultMatrix(activeAnt->getModelMatrix());

	glm::mat4 modelMatrix = antTransform.getGlobalTransformMatrix() * activeAnt->getModelMatrix();
	glm::mat4 viewMatrix = ofGetCurrentViewMatrix();
	glm::mat4 projectionMatrix = cam.getProjectionMatrix();
	glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelMatrix)));

	lightTextureShader.setUniformMatrix4f("modelMatrix", modelMatrix);
	lightTextureShader.setUniformMatrix4f("viewMatrix", viewMatrix);
	lightTextureShader.setUniformMatrix4f("projectionMatrix", projectionMatrix);
	lightTextureShader.setUniformMatrix3f("normalMatrix", normalMatrix);

	lightTextureShader.setUniformTexture("baseColorMap", baseColorTexture, 3);
	lightTextureShader.setUniformTexture("normalMap", normalMapTexture, 4);
	lightTextureShader.setUniformTexture("metallicMap", metallicTexture, 5);
	lightTextureShader.setUniformTexture("roughnessMap", roughnessTexture, 6);
	

	if (!isMaterial) {
		
		lightTextureShader.setUniform1i("isAnt", true);
		//activeAnt->disableMaterials();
		lightTextureShader.setUniform3f("upperTint", glm::vec3(upperColor.get().r / 255.0f, upperColor.get().g / 255.0f, upperColor.get().b / 255.0f));
		lightTextureShader.setUniform3f("bottomTint", glm::vec3(bottomColor.get().r / 255.0f, bottomColor.get().g / 255.0f, bottomColor.get().b / 255.0f));

		for (int i = 0; i < activeAnt->getMeshCount(); ++i) {
			activeAnt->getCurrentAnimatedMesh(i).draw();
		}
	}
	else {
		activeAnt->enableTextures();
		activeAnt->enableMaterials();

		antWithMaterial.drawFaces();
		
	}

	ofPopMatrix();

	lightTextureShader.end();
	ofDisableDepthTest();
}

void CustomSceneController::mousePressed(int x, int y, int button)
{
	if (button != OF_MOUSE_BUTTON_LEFT) return;

	Ray ray = createRayFromMouse(cam, x, y);
	glm::vec3 hit;
	uint64_t now = ofGetElapsedTimeMillis();

	bool isDoubleClick = (now - lastClickTime) < doubleClickDelay;
	bool isAtStart = cam.getPosition() == firstPos;

	if (isDoubleClick && isAtStart) {
		ofPlanePrimitive rightWallPlane;
		rightWallPlane.set(rightWall.getWidth(), rightWall.getHeight());
		rightWallPlane.setPosition(rightWall.getPosition());
		rightWallPlane.setOrientation(rightWall.getOrientationEuler());

		if (intersectRay(ray, rightWallPlane, hit)) {
			startCameraTransition(rightPos, rightTarget);
		}
		else if (intersectRay(ray, leftWall, hit)) {
			startCameraTransition(leftPos, leftTarget);
		}
		else if (intersectRay(ray, poster, hit)) {
			startCameraTransition(posterPos, posterTarget);
		}
	}

	if (resetButton.inside(x, y) && isDoubleClick) {
		resetCamera();
	}

	lastClickTime = now;
}

void CustomSceneController::keyPressed(int key)
{
	switch (key)
	{
	case 'r':
		reloadShaders();
		break;
	}
}

void CustomSceneController::startCameraTransition(glm::vec3 newPos, glm::vec3 newTarget) {
	startPos = cam.getPosition();
	startTarget = cam.getTarget().getPosition();
	endPos = newPos;
	endTarget = newTarget;

	transitionStartTime = ofGetElapsedTimef();
	isTransitioning = true;
}
void CustomSceneController::resetCamera()
{
	startCameraTransition(firstPos, firstTarget);
}
void CustomSceneController::onUseMaterial(bool& value)
{
	useMaterial = isMaterial = value;
	if (value) {
		noMaterial = false;
	}

}
void CustomSceneController::onNoMaterial(bool& value)
{
	noMaterial = value;
	if (value) {
		activeAnt = ants[0];
		isMaterial = useMaterial = false;
	}
}

void CustomSceneController::onBlueChanged(bool& value)
{
	antColor = value;
	if (value) {
		activeAnt = ants[0];
		redTint = false;
		greenTint = false;
		doubleTint = false;
	}

}
void CustomSceneController::onRedChanged(bool& value)
{
	normalColor = value;
	if (value) {
		activeAnt = ants[1];
		blueTint = false;
		greenTint = false;
		doubleTint = false;
	}

}
void CustomSceneController::onGreenChanged(bool& value)
{
	metallicColor = value;
	if (value) {
		activeAnt = ants[2];
		blueTint = false;
		redTint = false;
		doubleTint = false;
	}

}
void CustomSceneController::onDoubleChanged(bool& value)
{
	roughnessColor = value;
	if (value) {
		blueTint = false;
		redTint = false;
		greenTint = false;
	}

}
void CustomSceneController::onIdentityChanged(bool& value)
{
	identite = value;
	if (value) {
		aiguiser = border = bosseler = flou = false;
		ck = ConvolutionKernel::identity;
		applyFilterToImage(posterImg, posterFilter, getKernelFromEnum(ck), 3);
		filterActivated = true;
	}

}
void CustomSceneController::onSharpChanged(bool& value)
{
	aiguiser = value;
	if (value) {
		identite = border = bosseler = flou = false;
		ck = ConvolutionKernel::sharpen;
		applyFilterToImage(posterImg, posterFilter, getKernelFromEnum(ck), 3);
		filterActivated = true;
	}

}
void CustomSceneController::onBorderChanged(bool& value)
{
	border = value;
	if (value) {
		aiguiser = identite = bosseler = flou = false;
		ck = ConvolutionKernel::edge_detect;
		applyFilterToImage(posterImg, posterFilter, getKernelFromEnum(ck), 3);
		filterActivated = true;
	}

}
void CustomSceneController::onEmbossChanged(bool& value)
{
	bosseler = value;
	if (value) {
		aiguiser = border = identite = flou = false;
		ck = ConvolutionKernel::emboss;
		applyFilterToImage(posterImg, posterFilter, getKernelFromEnum(ck), 3);
		filterActivated = true;
	}

}
void CustomSceneController::onBlurChanged(bool& value)
{
	flou = value;
	if (value) {
		aiguiser = border = bosseler = identite = false;
		ck = ConvolutionKernel::blur;
		applyFilterToImage(posterImg, posterFilter, getKernelFromEnum(ck), 3);
		filterActivated = true;
	}

}
void CustomSceneController::onColorRightPick(bool& value)
{
	colorChoiceRight = value;
	if (value)
	{
		posterChoiceRight = false;
	}
}
void CustomSceneController::onColorLeftPick(bool& value)
{
	colorChoiceLeft = value;
	if (value)
	{
		posterPick = false;
	}
}
void CustomSceneController::onPosterSet(bool& value)
{
	if (!posterImg.isAllocated()) {
		posterSet = posterPick = false;
		return;
	}
	posterSet = posterPick = value;
	if (value) {
		colorChoiceLeft = false;
	}
}
void CustomSceneController::onTexturePick(bool& value)
{
	textureActivated = posterChoiceRight = value;
	if (value)
	{
		colorChoiceRight = false;
	}
}
void CustomSceneController::onWoodPick(bool& value)
{
	woodPick = value;
	if (value) {
		wallTexture = wood.getTexture();
		wallTexture.setTextureMinMagFilter(GL_LINEAR, GL_LINEAR);
		wallTexture.generateMipmap();
		brickPick = false;
	}
}
void CustomSceneController::onBrickPick(bool& value)
{
	brickPick = value;
	if (value) {
		wallTexture = brick.getTexture();
		wallTexture.setTextureMinMagFilter(GL_LINEAR, GL_LINEAR);
		wallTexture.generateMipmap();
		woodPick = false;
	}
}
void CustomSceneController::onReliefSelected(bool& value)
{
	reliefIsActivated = reliefActivatedRight = value;
	if (value)
	{
		activatedRelief(wallTexture, rightWall, planeMeshRight, woodPick ? wood : brick);
	}
	else {
		planeMeshRight = rightWall.getMesh();
	}
}
void CustomSceneController::applyFilterToImage(ofImage& sourceImage, ofImage& outputImage, const std::vector<float>& kernel, int kernelSize)
{
	if (!sourceImage.isAllocated()) return;

	int width = sourceImage.getWidth();
	int height = sourceImage.getHeight();
	int offset = kernelSize / 2;

	ofPixels& srcPixels = sourceImage.getPixels();
	ofPixels dstPixels;
	dstPixels.allocate(width, height, OF_IMAGE_COLOR);

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			float r = 0.0f, g = 0.0f, b = 0.0f;

			for (int ky = -offset; ky <= offset; ++ky)
			{
				for (int kx = -offset; kx <= offset; ++kx)
				{
					int px = ofClamp(x + kx, 0, width - 1);
					int py = ofClamp(y + ky, 0, height - 1);
					int kernelIndex = (ky + offset) * kernelSize + (kx + offset);

					ofColor c = srcPixels.getColor(px, py);
					float k = kernel[kernelIndex];

					r += c.r * k;
					g += c.g * k;
					b += c.b * k;
				}
			}

			dstPixels.setColor(x, y, ofColor(ofClamp(r, 0, 255), ofClamp(g, 0, 255), ofClamp(b, 0, 255)));
		}
	}

	outputImage.setFromPixels(dstPixels);
}

void CustomSceneController::openPosterChoicer()
{
	ofFileDialogResult result = ofSystemLoadDialog("Choisir une image");
	if (result.bSuccess) {
		posterImg.load(result.getPath());
		leftWall.mapTexCoordsFromTexture(posterImg.getTexture());
		posterSet = posterPick = true;
	}
	posterChoiceLeft = false;
}

void CustomSceneController::drawGUI()
{
	if (cam.getPosition() == leftPos)
	{
		guiLeft.draw();
	}
	else if (cam.getPosition() == rightPos) {
		guiRight.draw();
		controlPointsGui.draw();
	}
	else if (cam.getPosition() == firstPos) {
		gui.draw();
		tintGui.draw();
	}
	lightPanel.draw();
}

void CustomSceneController::activatedRelief(ofTexture& imgTexture, ofBoxPrimitive& box, ofMesh& boxMesh, ofImage grayscaleImg)
{
	grayscaleImg.setImageType(OF_IMAGE_GRAYSCALE);
	// Applique les coordonn�es UV � la bo�te (pour la texture)
	box.mapTexCoordsFromTexture(imgTexture);

	// R�cup�re les dimensions pour bien mapper les coordonn�es
	float width = box.getWidth();
	float height = box.getHeight();

	// R�cup�re une copie du mesh de la bo�te
	boxMesh = box.getMesh();

	for (int i = 0; i < boxMesh.getNumVertices(); ++i) {
		glm::vec3 v = boxMesh.getVertex(i);

		// Conversion des coordonn�es du sommet en coordonn�e d�image (u,v)
		float u = ofMap(v.x, -width / 2.0f, width / 2.0f, 0, grayscaleImg.getWidth() - 1);
		float vImg = ofMap(v.y, -height / 2.0f, height / 2.0f, 0, grayscaleImg.getHeight() - 1);

		// R�cup�re la luminosit� � cette position (image en niveaux de gris)
		float brightness = grayscaleImg.getColor(u, vImg).getLightness() / 255.0f;

		// Applique un d�placement selon la profondeur Z (face frontale)
		float bumpAmount = brightness * reliefStrength;

		v.z += bumpAmount * 0.5f;

		// Met � jour le sommet
		boxMesh.setVertex(i, v);
	}

	// Recalcule les UV
	boxMesh.clearTexCoords();
	boxMesh.addTexCoords(box.getMesh().getTexCoords());
}
void CustomSceneController::onControlPointYChanged(float& value)
{
	for (int i = 0; i < controlPointYSliders.size(); ++i) {
		controlPoints[i].y = controlPointYSliders[i];
	}
}
const std::vector<float>& CustomSceneController::getKernelFromEnum(ConvolutionKernel kernelType)
{
	static const std::vector<float> identity(convolution_kernel_identity.begin(), convolution_kernel_identity.end());
	static const std::vector<float> sharpen(convolution_kernel_sharpen.begin(), convolution_kernel_sharpen.end());
	static const std::vector<float> edge_detect(convolution_kernel_edge_detect.begin(), convolution_kernel_edge_detect.end());
	static const std::vector<float> emboss(convolution_kernel_emboss.begin(), convolution_kernel_emboss.end());
	static const std::vector<float> blur(convolution_kernel_blur.begin(), convolution_kernel_blur.end());

	switch (kernelType)
	{
	case ConvolutionKernel::identity:
		return identity;
	case ConvolutionKernel::sharpen:
		return sharpen;
	case ConvolutionKernel::edge_detect:
		return edge_detect;
	case ConvolutionKernel::emboss:
		return emboss;
	case ConvolutionKernel::blur:
		return blur;
	default:
		return identity;
	}
}

Ray CustomSceneController::createRayFromMouse(ofEasyCam& cam, int mouseX, int mouseY)
{
	glm::vec3 nearPoint = cam.screenToWorld(glm::vec3(mouseX, mouseY, 0));
	glm::vec3 farPoint = cam.screenToWorld(glm::vec3(mouseX, mouseY, 1));
	glm::vec3 dir = glm::normalize(farPoint - nearPoint);

	return { nearPoint, dir };
}

bool CustomSceneController::intersectRay(const Ray& ray, ofPlanePrimitive& plane, glm::vec3& hitPoint) {
	glm::vec3 planeNormal = glm::normalize(glm::vec3(plane.getGlobalTransformMatrix() * glm::vec4(0, 0, 1, 0)));
	glm::vec3 planePos = plane.getPosition();

	float denom = glm::dot(planeNormal, ray.direction);
	if (abs(denom) > 1e-6f) {
		float t = glm::dot(planePos - ray.origin, planeNormal) / denom;
		if (t >= 0) {
			hitPoint = ray.origin + t * ray.direction;

			// Transforme en espace local du plan pour test de collision
			glm::vec3 local = glm::inverse(plane.getGlobalTransformMatrix()) * glm::vec4(hitPoint, 1.0f);
			float halfW = plane.getWidth() * 0.5f;
			float halfH = plane.getHeight() * 0.5f;

			return abs(local.x) <= halfW && abs(local.y) <= halfH;
		}
	}
	return false;
}


