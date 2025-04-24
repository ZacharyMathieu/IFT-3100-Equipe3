#include "application.h"
#include <GLFW/glfw3.h>


//--------------------------------------------------------------
void Application::setup()
{
	ofSetWindowShape(WINDOW_WIDTH, WINDOW_HEIGHT);
	ofSetFrameRate(60);
	ofSetLogLevel(OF_LOG_VERBOSE);

	gridController.setup(0, MENU_HEIGHT, WINDOW_WIDTH / 2, WINDOW_HEIGHT - MENU_HEIGHT);
	sceneController.setup(0 + WINDOW_WIDTH / 2, MENU_HEIGHT, WINDOW_WIDTH / 2, WINDOW_HEIGHT - MENU_HEIGHT, &gridController);

	if (sablierimg.load("images/time-left.png")) {
		sablier.pixels = sablierimg.getPixels().getData();
		sablier.width = sablierimg.getWidth();
		sablier.height = sablierimg.getHeight();
		sablierCursor = glfwCreateCursor(&sablier, sablier.width, sablier.height);
	}
	else {
		ofLogError() << "Impossible de charger le curseur sablier !";
	}

	setupButtons();

	// GUI de l'efface
	eraserGui.setup("Efface");
	eraserSize.set("Taille de l'efface", 10, 1, 100);
	eraserGui.add(eraserSize);
	eraserSize.addListener(this, &Application::onEraserSizeChanged);
	eraserGui.setPosition(10, MENU_HEIGHT + 10);

	// GUI du crayon
	penGui.setup("Crayon");
	drawCursorSize.set("Taille du crayon", 5, 1, 50);
	penGui.add(drawCursorSize);
	drawCursorSize.addListener(this, &Application::onDrawCursorSizeChanged);

	wallPenChoice.set("Wall", true);
	wallPenChoice.addListener(this, &Application::onWallSelected);
	penChoice.add(wallPenChoice);

	foodPenChoice.set("Food", false);
	foodPenChoice.addListener(this, &Application::onFoodSelected);
	penChoice.add(foodPenChoice);

	penChoice.setName("Pen Choice");
	penGui.add(penChoice);
	penGui.setPosition(10, MENU_HEIGHT + 10);

	// GUI de la roue de couleur (seule)
	colorGui.setup("Couleur");
	tempColor.set("Couleur", ofColor(117, 255, 107), ofColor(0, 0), ofColor(255, 255));
	colorGui.add(tempColor);
	tempColor.addListener(this, &Application::onColorChanged);
	colorGui.setPosition(10, MENU_HEIGHT + 10);

	//GUI texture 
	textureGui.setup("Wall textures");
	brickPick.setName("Brick");
	rockPick.setName("Rock");
	metalPick.setName("Metal");
	blueMetalPick.setName("Blue Metal");
	textureGui.add(brickPick);
	textureGui.add(rockPick);
	textureGui.add(metalPick);
	textureGui.add(blueMetalPick);
	textureGui.setPosition(10, MENU_HEIGHT + 10);

	textureSelection.push_back(&brickPick);
	textureSelection.push_back(&rockPick);
	textureSelection.push_back(&metalPick);
	textureSelection.push_back(&blueMetalPick);

	for (auto* param : textureSelection) {
		param->addListener(this, &Application::onTextureSelected);
	}
	texturesToFalse();
	textureSelection[0]->set(true);

	gui.setup();

	gui.add(color_picker_ambient.set("ambient color", ofColor(63, 63, 63), ofColor(0, 0), ofColor(255, 255)));
	gui.add(color_picker_diffuse.set("diffuse color", ofColor(174, 223, 134), ofColor(0, 0), ofColor(255, 255)));

	cameraGui.setup();
	checkPop.setName("Deuxième vue");
	cameraChoice.setName("Camera choice");
	mainCamera.setName("Main Camera");
	topCamera.setName("Top Camera");
	freeCamera.setName("Free Camera");
	POVCamera.setName("POV Camera");
	cameraGui.add(checkPop);

	cameraChoice.add(mainCamera);
	cameraSelection.push_back(&mainCamera);
	cameraChoice.add(topCamera);
	cameraSelection.push_back(&topCamera);
	cameraChoice.add(freeCamera);
	cameraSelection.push_back(&freeCamera);
	cameraChoice.add(POVCamera);
	cameraSelection.push_back(&POVCamera);

	cameraGui.add(cameraChoice);

	checkPop = false;
	checkPop.addListener(this, &Application::onCheckPopChanged);

	cameraGui.setPosition(WINDOW_WIDTH / 2 - cameraGui.getWidth() - 10, MENU_HEIGHT + 10);

	for (auto* param : cameraSelection) {
		param->addListener(this, &Application::onCameraSelected);
	}
	camerasToFalse();
	cameraSelection[0]->set(true);
  
	guiTextureParameter.setup();;
	guiTextureParameter.add(material_brightness.set("brightness", 0.5, 0, 1));
	guiTextureParameter.add(material_metallic.set("metallic", 0.5, 0, 1));
	guiTextureParameter.add(material_roughness.set("roughness", 0.5, 0, 1));
	guiTextureParameter.add(material_occlusion.set("occlusion", 1, 0, 1));

	guiTextureParameter.setPosition(10 + textureGui.getWidth(), MENU_HEIGHT +10);
}

//--------------------------------------------------------------
void Application::setupButtons()
{
	importImageIcon.load("images/importImage.png");
	exportImageIcon.load("images/exportImage.png");
	playIcon.load("images/play.png");
	pauseIcon.load("images/pause.png");
	fastForwardIcon.load("images/fastForward.png");
	eraseModeIcon.load("images/eraseMode.png");
	drawModeIcon.load("images/drawMode.png");
	penTypeChoiceIcon.load("images/penTypeChoice.png");
	selectIcon.load("images/select.png");
	textureIcon.load("images/texture.png");
	undoIcon.load("images/undo.png");
	redoIcon.load("images/redo.png");
	cameraIcon.load("images/camera.png");
	antIcon.load("images/ant.png");

	vector<std::tuple<Button*, void (Application::*)(), ofImage*>> buttonMap = {
		std::tuple(&importImageButton, &Application::importImage, &importImageIcon),
		std::tuple(&exportImageButton, &Application::exportImage, &exportImageIcon),
		std::tuple(&playButton, &Application::play, &playIcon),
		std::tuple(&fastForwardButton, &Application::fastForward, &fastForwardIcon),
		std::tuple(&eraseModeButton, &Application::eraseMode, &eraseModeIcon),
		std::tuple(&drawModeButton, &Application::drawMode, &drawModeIcon),
		std::tuple(&penTypeChoiceButton, &Application::penTypeChoice, &penTypeChoiceIcon),
		std::tuple(&selectButton, &Application::multipleSelection, &selectIcon),
		std::tuple(&textureButton, &Application::textureChoice, &textureIcon),
		std::tuple(&undoButton, &Application::undo, &undoIcon),
		std::tuple(&redoButton, &Application::redo, &redoIcon),
		std::tuple(&antButton, &Application::customAnt, &antIcon),
		std::tuple(&cameraButton, &Application::cameraMode, &cameraIcon)
	};

	int i = 0;
	for (auto bTuple : buttonMap)
	{
		int xPos = i * (MENU_BUTTON_WIDTH + MENU_BUTTON_MARGIN);
		if (std::get<0>(bTuple) == &cameraButton) {
			std::get<0>(bTuple)->setup(WINDOW_WIDTH / 2, 0, MENU_BUTTON_WIDTH, MENU_HEIGHT, this, std::get<1>(bTuple), std::get<2>(bTuple));
		}
		else {
			std::get<0>(bTuple)->setup(xPos, 0, MENU_BUTTON_WIDTH, MENU_HEIGHT, this, std::get<1>(bTuple), std::get<2>(bTuple));
		}

		buttons.push_back(std::get<0>(bTuple));
		i++;
	}
}

//--------------------------------------------------------------
void Application::drawMenu()
{
	ofSetBackgroundColor(255);
	ofSetColor(0);
	for (Button* b : buttons)
	{
		ofSetColor(255);
		b->draw();
	}
	ofDrawLine(0, MENU_HEIGHT, WINDOW_WIDTH, MENU_HEIGHT);
}

//--------------------------------------------------------------
void Application::update()
{
	gui.loadFont("verdana.ttf", 12);
	cameraGui.loadFont("verdana.ttf", 12);
	textureGui.loadFont("verdana.ttf", 12);
	penGui.loadFont("verdana.ttf", 12);
	eraserGui.loadFont("verdana.ttf", 12);
	colorGui.loadFont("verdana.ttf", 12);

	sceneController.COLOR_AMBIENT = color_picker_ambient;
	sceneController.COLOR_DIFFUSE = color_picker_diffuse;
	gridController.foodColor = tempColor;

	sceneController.material_brightness = material_brightness;
	sceneController.material_metallic = material_metallic;
	sceneController.material_roughness = material_roughness;
	sceneController.material_occlusion = material_occlusion;

	sceneController.textureSelected = showTextureParameterMenu;

	sceneController.update();

	if (isRunning) {
		gridController.update();
	}

	sceneController.updateCellPositions();
	sceneController.updateAntPositions();
}

//--------------------------------------------------------------
void Application::draw()
{
	drawMenu();

	gridController.draw(sceneController.ant);
	

	if (showDrawMenu)
		penGui.draw();
	if (showEraserMenu)
		eraserGui.draw();
	if (showColorMenu)
		colorGui.draw();
	if (showTextureMenu)
		textureGui.draw();
	if (showTextureMenu)
		guiTextureParameter.draw();
	if (showCameraMenu)
		cameraGui.draw();

	drawCustomCursors();
	sceneController.draw();
}

//--------------------------------------------------------------
void Application::drawCustomCursors()
{
	ofSetColor(0);

	// Si la souris est dans le menu, afficher le curseur par défaut
	if (ofGetMouseY() < MENU_HEIGHT)
	{
		ofShowCursor();
	}
	// Si la souris est dans le menu de l'efface, afficher le curseur par défaut
	else if (ofGetMouseX() > COORD_MENU_X
		&& ofGetMouseX() < OPENED_MENU_WIDTH
		&& ofGetMouseY() > COORD_MENU_Y
		&& ofGetMouseY() < COORD_MENU_Y + OPENED_MENU_HEIGHT_ERASE
		&& cursorMode == ERASE)
	{
		ofShowCursor();
	}
	// Si la souris est dans le menu du crayon, afficher le curseur par défaut
	else if (ofGetMouseX() > COORD_MENU_X
		&& ofGetMouseX() < OPENED_MENU_WIDTH
		&& ofGetMouseY() > COORD_MENU_Y
		&& ofGetMouseY() < COORD_MENU_Y + OPENED_MENU_HEIGHT_DRAW
		&& cursorMode == DRAW)
	{
		ofShowCursor();
	}
	else
	{
		// Dessiner le curseur du crayon
		if (cursorMode == DRAW && ofGetMouseX() < ofGetWidth() / 2 && ofGetMouseY() > MENU_HEIGHT)
		{
			int cursorSize = drawCursorSize;
			ofSetColor(currentDrawColor);
			ofDrawLine(ofGetMouseX() - cursorSize, ofGetMouseY(), ofGetMouseX() + cursorSize, ofGetMouseY());
			ofDrawLine(ofGetMouseX(), ofGetMouseY() - cursorSize, ofGetMouseX(), ofGetMouseY() + cursorSize);
			ofHideCursor();
		}
		// Dessiner le curseur de l'efface
		else if (cursorMode == ERASE && ofGetMouseX() < ofGetWidth() / 2 && ofGetMouseY() > MENU_HEIGHT)
		{
			ofNoFill();
			ofDrawCircle(ofGetMouseX(), ofGetMouseY(), eraserSize);
			ofFill();
			ofHideCursor();
		}
		else if (cursorMode == SELECT && ofGetMouseX() < ofGetWidth() / 2 && ofGetMouseY() > MENU_HEIGHT) {
			ofShowCursor();
			SetCursor(LoadCursor(NULL, IDC_HAND));
		}
		else if (ofGetMouseX() > ofGetWidth() / 2 && ofGetMouseY() > MENU_HEIGHT)
		{
			ofShowCursor();
			SetCursor(LoadCursor(NULL, IDC_CROSS));
		}
		else if (cursorMode == DEFAULT) {
			ofShowCursor();
			SetCursor(LoadCursor(NULL, IDC_ARROW));
		}
	}
}

//--------------------------------------------------------------
void Application::onColorChanged(ofColor& color)
{
	currentDrawColor = color;
	gridController.foodColor = color;
	
}

//--------------------------------------------------------------
void Application::onDrawCursorSizeChanged(int& size)
{
	drawCursorSize = size;
}

//--------------------------------------------------------------
void Application::onEraserSizeChanged(int& size)
{
	eraserSize = size;
}

//--------------------------------------------------------------
void Application::exit()
{
}

//--------------------------------------------------------------
void Application::keyPressed(int key)
{
	sceneController.keyPressed(key);

	if (key == 'z') {
		gridController.keyPressed(key);
	}
	if (key == 'y') {
		gridController.keyPressed(key);
	}
	if (key == 'c')
	{
		changeCameraSelected(sceneController.numCam);
	}
}

//--------------------------------------------------------------
void Application::keyReleased(int key)
{
}

//--------------------------------------------------------------
void Application::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void Application::mouseDragged(int x, int y, int button)
{
	gridController.mouse_current_x = x;
	gridController.mouse_current_y = y;
	string cursor;
	CellType material = WALL;

	switch (cursorMode)
	{
	case DRAW:
		cursor = "DRAW";
		if (wallPenChoice)
			material = WALL;
		else if (foodPenChoice)
			material = FOOD;
		break;
	case ERASE:
		cursor = "ERASE";
		break;
	case SELECT:
		cursor = "SELECT";
		break;
	default:

		break;
	}

	gridController.mouseDragged(x, y, button, cursor, material, drawCursorSize, eraserSize);
}

//--------------------------------------------------------------
void Application::mousePressed(int x, int y, int button)
{
	gridController.mouse_pressed_x = x;
	gridController.mouse_pressed_y = y;
	gridController.mouse_current_x = x;
	gridController.mouse_current_y = y;

	if (y < MENU_HEIGHT && x < (MENU_BUTTON_WIDTH + MENU_BUTTON_MARGIN) * (buttons.size() - 1) || y < MENU_HEIGHT && x < MENU_BUTTON_WIDTH + (WINDOW_WIDTH / 2) && x > WINDOW_WIDTH / 2)
	{
		int buttonNumber = x / (MENU_BUTTON_WIDTH + MENU_BUTTON_MARGIN);
		if (buttonNumber > buttons.size() - 1) buttonNumber = buttons.size() - 1;
		Button* pressedButton = buttons[buttonNumber];

		// Toggle (ouverture/fermeture) pour chaque menu
		if (pressedButton == &eraseModeButton)
		{
			showEraserMenu = !showEraserMenu;
			showDrawMenu = false;
			showColorMenu = false;
			showCameraMenu = false;
			showTextureMenu = false;
			showTextureParameterMenu = false;
			cursorMode = showEraserMenu ? ERASE : DEFAULT;
			return;
		}
		if (pressedButton == &drawModeButton)
		{
			showDrawMenu = !showDrawMenu;
			showTextureMenu = false;
			showEraserMenu = false;
			showColorMenu = false;
			showTextureParameterMenu = false;
			showCameraMenu = false;
			cursorMode = showDrawMenu ? DRAW : DEFAULT;

			return;
		}
		if (pressedButton == &penTypeChoiceButton)
		{
			showColorMenu = !showColorMenu;
			showDrawMenu = false;
			showTextureMenu = false;
			showEraserMenu = false;
			showCameraMenu = false;
			showTextureParameterMenu = false;
			cursorMode = DEFAULT;
			return;
		}
		if (pressedButton == &textureButton)
		{
			showTextureMenu = !showTextureMenu;
			showTextureParameterMenu = !showTextureParameterMenu;

			showDrawMenu = false;
			showEraserMenu = false;
			showColorMenu = false;
			showCameraMenu = false;
			cursorMode = DEFAULT;
			return;

		}
		if (pressedButton == &cameraButton)
		{
			showCameraMenu = !showCameraMenu;
			showDrawMenu = false;
			showEraserMenu = false;
			showColorMenu = false;
			showTextureMenu = false;
			showTextureParameterMenu = false;
			cursorMode = DEFAULT;

			return;

		}
		if (pressedButton == &antButton) {
			showCameraMenu = false;
			showDrawMenu = false;
			showEraserMenu = false;
			showColorMenu = false;
			showTextureMenu = false;
			showTextureParameterMenu = false;
			cursorMode = DEFAULT;
			customAnt();
			return;
		}



		// Si on clique ailleurs, tout fermer
		showDrawMenu = false;
		showEraserMenu = false;
		showColorMenu = false;
		showTextureMenu = false;
		showTextureMenu = false;

		if (pressedButton == &penTypeChoiceButton)
		{
			showColorMenu = !showColorMenu;
			return;
		}
		pressedButton->mousePressed(x, y, button);
	}

	if (showEraserMenu && y >= 85 && y <= 95)
	{
		if (x >= COORD_SLIDER_X && x <= COORD_SLIDER_X + SLIDER_WIDTH)
		{
			eraserSize = (x - COORD_SLIDER_X) * 50 / SLIDER_WIDTH;
		}
	}

	if (showDrawMenu)
	{
		if (y >= 85 && y <= 95)
		{
			if (x >= COORD_SLIDER_X && x <= COORD_SLIDER_X + SLIDER_WIDTH)
			{
				drawCursorSize = (x - COORD_SLIDER_X) * 50 / SLIDER_WIDTH;
			}
		}

		int centerX = 110, centerY = 170, radius = 60;
		int dx = ofGetMouseX() - centerX;
		int dy = ofGetMouseY() - centerY;
		float distance = sqrt(dx * dx + dy * dy);

		if (distance <= radius)
		{
			float angle = atan2(dy, dx) * RAD_TO_DEG;
			if (angle < 0)
				angle += 360;
			currentDrawColor = ofColor::fromHsb(angle / 360.0 * 255, 255, 255);
		}
	}

	if (showColorMenu)
	{
		int centerX = 110, centerY = 190, radius = 60;
		int dx = ofGetMouseX() - centerX;
		int dy = ofGetMouseY() - centerY;
		float distance = sqrt(dx * dx + dy * dy);

		if (distance <= radius)
		{
			float angle = atan2(dy, dx) * RAD_TO_DEG;
			if (angle < 0)
				angle += 360;
			currentDrawColor = ofColor::fromHsb(angle / 360.0 * 255, 255, 255);
		}
	}

	if (showTextureMenu)
	{

	}

	// Vérifier si on clique sur la flêche du menu du crayon
	if (showDrawMenu && x >= (10 + (isDrawMenuCollapsed ? 20 : 200)) - 10 &&
		x <= (20 + (isDrawMenuCollapsed ? 30 : 200)) &&
		y >= 60 && y <= 75)
	{
		isDrawMenuCollapsed = !isDrawMenuCollapsed;
		return;
	}

	// Vérifier si on clique sur la flêche du menu de l'effaceur
	if (showEraserMenu && x >= (10 + (isEraserMenuCollapsed ? 20 : 200)) - 10 &&
		x <= (20 + (isEraserMenuCollapsed ? 30 : 200)) &&
		y >= 60 && y <= 75)
	{
		isEraserMenuCollapsed = !isEraserMenuCollapsed;
		return;
	}

	// Vérifier si on clique sur la flêche du menu de sélection de couleur
	if (showColorMenu && x >= (10 + (isColorMenuCollapsed ? 20 : 200)) - 10 &&
		x <= (20 + (isColorMenuCollapsed ? 30 : 200)) &&
		y >= 60 && y <= 75)
	{
		isColorMenuCollapsed = !isColorMenuCollapsed;
		return;
	}

	if (showTextureMenu && x >= (10 + (isTextureMenuCollapsed ? 20 : 200)) - 10 &&
		x <= (20 + (isTextureMenuCollapsed ? 30 : 200)) &&
		y >= 60 && y <= 75)
	{
		isColorMenuCollapsed = !isColorMenuCollapsed;
		return;
	}
	string cursor;

	switch (cursorMode)
	{
	case DRAW:
		cursor = "DRAW";
		break;
	case ERASE:
		cursor = "ERASE";
		break;
	case SELECT:
		cursor = "SELECT";
		break;
	default:
		cursor = "DEFAULT";
		break;
	}
	ofLog() << cursorMode;
	gridController.mousePressed(x, y, button, cursor);
}

//--------------------------------------------------------------
void Application::mouseReleased(int x, int y, int button)
{
	string action;
	if (isDrawMenuCollapsed) {
		
		if (foodPenChoice) action = "FOOD";
		else action = "DRAW";
	}
	else if(isEraserMenuCollapsed)
	{
		action = "ERASE";
	}
	gridController.mouseReleased(x, y, button, action);
}

//--------------------------------------------------------------
void Application::mouseScrolled(int x, int y, float scrollX, float scrollY)
{
}

//--------------------------------------------------------------
void Application::mouseEntered(int x, int y)
{
}

//--------------------------------------------------------------
void Application::mouseExited(int x, int y)
{
}

//--------------------------------------------------------------
void Application::windowResized(int w, int h)
{
}

//--------------------------------------------------------------
void Application::gotMessage(ofMessage msg)
{
}

//--------------------------------------------------------------
void Application::dragEvent(ofDragInfo dragInfo)
{
}

//--------------------------------------------------------------
void Application::importImage()
{
	ofFileDialogResult result = ofSystemLoadDialog("Importer une image");
	if (result.bSuccess)
	{
		string filePath = result.getPath();
		ofFile file(filePath);

		ofLog() << "Tentative de chargement de l'image : " << filePath;

		if (importedImage.load(filePath))
		{
			imageLoaded = true;
			ofLog() << "Image importée avec succès : " << filePath;
			ofLog() << "Taille de l'image : " << importedImage.getWidth() << "x" << importedImage.getHeight();
		}
		else
		{
			ofLogError() << "Échec du chargement de l'image.";
			imageLoaded = false;
		}
		gridController.importGrid(file);

		sceneController.updateGridController(&gridController);
	}
}

//--------------------------------------------------------------
void Application::exportImage()
{
	cursorMode = DEFAULT;
	std::string defaultPath = ofFilePath::getUserHomeDir() + "/capture.png";
	ofFileDialogResult saveFile = ofSystemSaveDialog(defaultPath, "save grid");

	if (saveFile.bSuccess)
	{
		createColorCanva(saveFile.getPath());
	}

	cout << "exportImage\n";
}

//--------------------------------------------------------------
void Application::play()
{
	cursorMode = DEFAULT;
	isRunning = !isRunning;
	if (isRunning)
	{
		playButton.setIcon(&pauseIcon);
		sceneController.animation = true;
		sceneController.playMode = true;
	}
	else
	{
		playButton.setIcon(&playIcon);
		sceneController.animation = false;
		sceneController.playMode = false;
	}
}

//--------------------------------------------------------------
void Application::fastForward()
{
	cursorMode = DEFAULT;
	for (int _ = 0; _ < FAST_FORWARD_AMOUNT; _++) {
		gridController.update();
	}
}

//--------------------------------------------------------------
void Application::eraseMode()
{
	cursorMode = ERASE;
	ofHideCursor();

	showEraserMenu = true;
	showDrawMenu = false;
	showColorMenu = false;
	showTextureMenu = false;
}

//--------------------------------------------------------------
void Application::drawMode()
{
	cursorMode = DRAW;
	ofHideCursor();

	showDrawMenu = true;
	showEraserMenu = false;
	showTextureMenu = false;

	showColorMenu = true; // Affiche la roue de sélection de couleur automatiquement
}

//--------------------------------------------------------------
void Application::penTypeChoice()
{
	cursorMode = DEFAULT;
	showColorMenu = !showColorMenu;
	showEraserMenu = false;
	showDrawMenu = false;
	showTextureMenu = false;
	ofShowCursor();
}

void Application::multipleSelection()
{
	cursorMode = SELECT;

	ofShowCursor();

	showEraserMenu = false;
	showDrawMenu = false;
	showColorMenu = false;
	showTextureMenu = false;

}

void Application::textureChoice()
{
	cursorMode = DEFAULT;
	ofShowCursor;
	showTextureMenu = true;
	showEraserMenu = false;
	showDrawMenu = false;
	showColorMenu = false;
}

void Application::texturesToFalse()
{
	for (int i = 0; i < textureSelection.size(); i++) {
		textureSelection[i]->set(false);
	}
}
void Application::camerasToFalse()
{
	for (int i = 0; i < cameraSelection.size(); i++) {
		cameraSelection[i]->set(false);
	}
}

void Application::onTextureSelected(bool& value)
{
	if (value) {
		int x = 0;
		for (auto* param : textureSelection) {
			if (&(param->get()) != &value) {  
				param->set(false);
				x++;
			}
			else {
				switch (x) {
				case 0:
					sceneController.texture_albedo.load(sceneController.brickTexture[0]);
					sceneController.texture_normal.load(sceneController.brickTexture[1]);
					sceneController.texture_arm.load(sceneController.brickTexture[2]);
					break;
				case 1:
					sceneController.texture_albedo.load(sceneController.rockTexture[0]);
					sceneController.texture_normal.load(sceneController.rockTexture[1]);
					sceneController.texture_arm.load(sceneController.rockTexture[2]);
					break;
				case 2:
					sceneController.texture_albedo.load(sceneController.metalTexture[0]);
					sceneController.texture_normal.load(sceneController.metalTexture[1]);
					sceneController.texture_arm.load(sceneController.metalTexture[2]);
					break;
				case 3:
					sceneController.texture_albedo.load(sceneController.blueMetalTexture[0]);
					sceneController.texture_normal.load(sceneController.blueMetalTexture[1]);
					sceneController.texture_arm.load(sceneController.blueMetalTexture[2]);
					break;
				default:
					break;
				}
			}
		}
	}
}

void Application::onCameraSelected(bool& value)
{
	if (value) {
		int x = 0;
		for (auto* param : cameraSelection) {
			if (&(param->get()) != &value) {  // comparer les adresses
				param->set(false);
				x++;
			}
			else
			{
				sceneController.activeCam = sceneController.cameras[x];
				sceneController.numCam = x;
			}
		}
	}
}

void Application::onWallSelected(bool& value)
{
	if (value || !foodPenChoice)
	{
		wallPenChoice.set(true);
		foodPenChoice.set(false);
	}
}

void Application::onFoodSelected(bool& value)
{
	if (value || !wallPenChoice)
	{
		foodPenChoice.set(true);
		wallPenChoice.set(false);
	}
}

void Application::undo()
{
	if (!gridController.Undo.empty())
		gridController.undo();
}

void Application::redo()
{
	if (!gridController.Redo.empty())
		gridController.redo();
}

void Application::cameraMode()
{
	//sceneController.checkPop = this->checkPop ? true : false;
	cursorMode = DEFAULT;
	showCameraMenu = !showCameraMenu;
	showEraserMenu = false;
	showDrawMenu = false;
	showTextureMenu = false;
	ofShowCursor();

}

//void Application::wallPosition3D()
//{
//
//	float size = sceneController.wallSize;
//
//	float sizeBoxX = gridController.scaleX * size;
//	float sizeBoxY = gridController.scaleY * size;
//
//	sceneController.positions.clear();
//
//	for (int y = 0; y < gridController.grid.grid.size(); y++) {
//		for (int x = 0; x < gridController.grid.grid[y].size(); x++) {
//			Cell* cell = gridController.grid.grid[y][x];
//			if (cell->type == WALL) {
//
//				if (abs(sceneController.boxCollider.getPosition().x - ((x * sizeBoxX) + (sizeBoxX / 2))) < (sizeBoxX * 1.5f) / 2
//					&& abs(sceneController.boxCollider.getPosition().z - ((y * sizeBoxY) + (sizeBoxY / 2))) < (sizeBoxY * 1.5) / 2)
//					continue;
//
//				glm::vec3 cubePosition((x * sizeBoxX) + (sizeBoxX / 2), 25, (y * sizeBoxY) + (sizeBoxY / 2));
//
//				sceneController.positions.push_back(cubePosition);
//			}
//		}
//	}
//}


void Application::createColorCanva(string filepath)
{
	ofFile file(filepath, ofFile::WriteOnly);

	for (int y = 0; y < gridController.grid.h; y++) {
		for (int x = 0; x < gridController.grid.w; x++) {

			float value = gridController.grid.at(x, y)->value;
			string type = gridController.grid.at(x, y)->type == WALL ? "WALL" : "PHEROMONE";

			file << x << ","
				<< y << ","
				<< value << ","
				<< type << "\n";
		}
	}
	file << "ant" << "\n";
	for (Ant* ant : gridController.ants)
		file << ant->pos.x << "," << ant->pos.y << "\n";
	file.close();

}
void Application::onCheckPopChanged(bool& value) {
	sceneController.checkPop = value;
}

void Application::changeCameraSelected(int num)
{
	for (int i = 0; i < cameraSelection.size(); i++)
	{
		if (i == num) cameraSelection[i]->set(true);
		else cameraSelection[i]->set(false);
	}
}

void Application::customAnt()
{
	auto glfwWindow = dynamic_pointer_cast<ofAppGLFWWindow>(ofGetMainLoop()->getCurrentWindow());

	if (!antWindow) {
		GLFWwindow* native = glfwWindow->getGLFWWindow(); 
		glfwSetCursor(native, sablierCursor);
		ofGLFWWindowSettings settings;
		settings.setSize(1024, 612);
		settings.setPosition(ofVec2f(300, 300));
		settings.resizable = true;
		settings.setGLVersion(3, 3);

		antWindow = ofCreateWindow(settings);
		antApp = make_shared<CustomSceneController>();
		ofRunApp(antWindow, antApp);
		ofAddListener(antWindow->events().exit, this, &Application::onAntWindowClosed);

		ofSleepMillis(3000);
		glfwSetCursor(native, glfwCreateStandardCursor(GLFW_ARROW_CURSOR));

	}
}
void Application::onAntWindowClosed(ofEventArgs& args)
{
	antWindow.reset();

}



//void Application::antPosition3D()
//{
//	float size = sceneController.wallSize;
//
//	float sizeBoxX = gridController.scaleX * size;
//	float sizeBoxY = gridController.scaleY * size;
//
//	sceneController.antPositions.clear();
//
//	for (Ant* ant : gridController.ants)
//	{
//		ofPoint posAnt;
//		posAnt = ant->pos;
//		glm::vec3 antPosition((posAnt.x * sizeBoxX) + (sizeBoxX / 2), 5, (posAnt.y * sizeBoxY) + (sizeBoxY / 2));
//
//		sceneController.antPositions.push_back(antPosition);
//	}
//}
