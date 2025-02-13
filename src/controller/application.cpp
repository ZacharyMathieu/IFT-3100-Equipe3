#include "application.h"

//--------------------------------------------------------------
void Application::setup()
{
    ofSetWindowShape(WINDOW_WIDTH, WINDOW_HEIGHT);
    gridController.setup(0, MENU_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT - MENU_HEIGHT);
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
    colorPicker.set("Couleur", ofColor(255, 0, 0), ofColor(0, 0), ofColor(255, 255));
    penGui.add(colorPicker);
    penGui.add(drawCursorSize);
    colorPicker.addListener(this, &Application::onColorChanged);
    drawCursorSize.addListener(this, &Application::onDrawCursorSizeChanged);
    penGui.setPosition(10, MENU_HEIGHT + 10);

    // GUI de la roue de couleur (seule)
    colorGui.setup("Couleur");
    ofParameter<ofColor> tempColor;
    tempColor.set("Couleur", ofColor(255, 0, 0), ofColor(0, 0), ofColor(255, 255));
    colorGui.add(tempColor);
    tempColor.addListener(this, &Application::onColorChanged);
    colorGui.setPosition(10, MENU_HEIGHT + 10);
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
    shapeModeIcon.load("images/shapeMode.png");
    penTypeChoiceIcon.load("images/penTypeChoice.png");
    shapeChoiceIcon.load("images/shapeChoice.png");

    vector<std::tuple<Button *, void (Application::*)(), ofImage *>> buttonMap = {
        std::tuple(&importImageButton, &Application::importImage, &importImageIcon),
        std::tuple(&exportImageButton, &Application::exportImage, &exportImageIcon),
        std::tuple(&playButton, &Application::play, &playIcon),
        std::tuple(&fastForwardButton, &Application::fastForward, &fastForwardIcon),
        std::tuple(&eraseModeButton, &Application::eraseMode, &eraseModeIcon),
        std::tuple(&drawModeButton, &Application::drawMode, &drawModeIcon),
        std::tuple(&shapeModeButton, &Application::shapeMode, &shapeModeIcon),
        std::tuple(&penTypeChoiceButton, &Application::penTypeChoice, &penTypeChoiceIcon),
        std::tuple(&shapeChoiceButton, &Application::shapeChoice, &shapeChoiceIcon)};

    int i = 0;
    for (auto bTuple : buttonMap)
    {
        int xPos = i * (MENU_BUTTON_WIDTH + MENU_BUTTON_MARGIN);
        std::get<0>(bTuple)->setup(xPos, 0, MENU_BUTTON_WIDTH, MENU_HEIGHT, this, std::get<1>(bTuple), std::get<2>(bTuple));
        buttons.push_back(std::get<0>(bTuple));
        i++;
    }
}

//--------------------------------------------------------------
void Application::drawMenu()
{
    ofSetBackgroundColor(255);
    ofSetColor(0);
    for (Button *b : buttons)
    {
        ofSetColor(255);
        b->draw();
    }
    ofDrawLine(0, MENU_HEIGHT, WINDOW_WIDTH, MENU_HEIGHT);
}

//--------------------------------------------------------------
void Application::update()
{
}

//--------------------------------------------------------------
void Application::draw()
{
    drawMenu();

    if (imageLoaded)
    {
        ofSetColor(255);
        importedImage.draw(0, MENU_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT - MENU_HEIGHT);
    }
    gridController.draw();

    if (showDrawMenu) penGui.draw();
    if (showEraserMenu) eraserGui.draw();
    if (showColorMenu) colorGui.draw();

    drawCustomCursors();
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
    else
    {
        // Dessiner le curseur du crayon
        if (cursorMode == DRAW)
        {
            int cursorSize = drawCursorSize;
            ofSetColor(currentDrawColor);
            ofDrawLine(ofGetMouseX() - cursorSize, ofGetMouseY(), ofGetMouseX() + cursorSize, ofGetMouseY());
            ofDrawLine(ofGetMouseX(), ofGetMouseY() - cursorSize, ofGetMouseX(), ofGetMouseY() + cursorSize);
            ofHideCursor();
        }
        // Dessiner le curseur de l’efface
        else if (cursorMode == ERASE)
        {
            ofNoFill();
            ofDrawCircle(ofGetMouseX(), ofGetMouseY(), eraserSize);
            ofFill();
            ofHideCursor();
        }
        else
        {
            ofShowCursor();
        }
    }
}

//--------------------------------------------------------------
void Application::onColorChanged(ofColor &color)
{
    currentDrawColor = color;
}

//--------------------------------------------------------------
void Application::onDrawCursorSizeChanged(int &size)
{
    drawCursorSize = size;
}

//--------------------------------------------------------------
void Application::onEraserSizeChanged(int &size)
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
}

//--------------------------------------------------------------
void Application::mousePressed(int x, int y, int button)
{
    if (y < MENU_HEIGHT)
    {
        int buttonNumber = x / (MENU_BUTTON_WIDTH + MENU_BUTTON_MARGIN);

        // Toggle (ouverture/fermeture) pour chaque menu
        if (buttonNumber == 4) { showEraserMenu = !showEraserMenu; showDrawMenu = false; showColorMenu = false; cursorMode = showEraserMenu ? ERASE : DEFAULT; return; }
        if (buttonNumber == 5) { showDrawMenu = !showDrawMenu; showEraserMenu = false; showColorMenu = false; cursorMode = showDrawMenu ? DRAW : DEFAULT; return; }
        if (buttonNumber == 7) { showColorMenu = !showColorMenu; showDrawMenu = false; showEraserMenu = false; return; }

        // Si on clique ailleurs, tout fermer
        showDrawMenu = false;
        showEraserMenu = false;
        showColorMenu = false;
    }
}

//--------------------------------------------------------------
void Application::mouseReleased(int x, int y, int button)
{
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
        std::string path = saveFile.getPath();
        path += ".png";
        ofImage screenshot;
        screenshot.grabScreen(0, MENU_HEIGHT, ofGetWidth(), ofGetHeight() - MENU_HEIGHT);

        screenshot.save(path);
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
    }
    else
    {
        playButton.setIcon(&playIcon);
    }
}

//--------------------------------------------------------------
void Application::fastForward()
{
    cursorMode = DEFAULT;
}

//--------------------------------------------------------------
void Application::eraseMode()
{
    cursorMode = ERASE;
    ofHideCursor();

    showEraserMenu = true;
    showDrawMenu = false;
    showColorMenu = false;
}

//--------------------------------------------------------------
void Application::drawMode()
{
    cursorMode = DRAW;
    ofHideCursor();

    showDrawMenu = true;
    showEraserMenu = false;

    showColorMenu = true;  // Affiche la roue de sélection de couleur automatiquement
}

//--------------------------------------------------------------
void Application::shapeMode()
{
    cursorMode = DEFAULT;
    ofShowCursor();
}

//--------------------------------------------------------------
void Application::penTypeChoice()
{
    cursorMode = DRAW;
    showColorMenu = !showColorMenu;
    showEraserMenu = false;
    showDrawMenu = false;
    ofShowCursor();
}

//--------------------------------------------------------------
void Application::shapeChoice()
{
    // TODO
    cursorMode = DEFAULT;
}