#include "application.h"

void Application::setup()
{
    ofSetWindowShape(WINDOW_WIDTH, WINDOW_HEIGHT);
    grid.setup(0, MENU_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT);
    setupButtons();
}

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

    for (int i = 0; i < 9; i++) {
        int xPos = i * (MENU_BUTTON_WIDTH + BUTTON_MARGIN);
        switch (i) {
            case 0: importImageButton.setup(xPos, 0, MENU_BUTTON_WIDTH, MENU_HEIGHT, this, &Application::importImage, importImageIcon); break;
            case 1: exportImageButton.setup(xPos, 0, MENU_BUTTON_WIDTH, MENU_HEIGHT, this, &Application::exportImage, exportImageIcon); break;
            case 2: playButton.setup(xPos, 0, MENU_BUTTON_WIDTH, MENU_HEIGHT, this, &Application::play, playIcon); break;
            case 3: fastForwardButton.setup(xPos, 0, MENU_BUTTON_WIDTH, MENU_HEIGHT, this, &Application::fastForward, fastForwardIcon); break;
            case 4: eraseModeButton.setup(xPos, 0, MENU_BUTTON_WIDTH, MENU_HEIGHT, this, &Application::eraseMode, eraseModeIcon); break;
            case 5: drawModeButton.setup(xPos, 0, MENU_BUTTON_WIDTH, MENU_HEIGHT, this, &Application::drawMode, drawModeIcon); break;
            case 6: shapeModeButton.setup(xPos, 0, MENU_BUTTON_WIDTH, MENU_HEIGHT, this, &Application::shapeMode, shapeModeIcon); break;
            case 7: penTypeChoiceButton.setup(xPos, 0, MENU_BUTTON_WIDTH, MENU_HEIGHT, this, &Application::penTypeChoice, penTypeChoiceIcon); break;
            case 8: shapeChoiceButton.setup(xPos, 0, MENU_BUTTON_WIDTH, MENU_HEIGHT, this, &Application::shapeChoice, shapeChoiceIcon); break;
        }
    }

    buttons = {
        &importImageButton,
        &exportImageButton,
        &playButton,
        &fastForwardButton,
        &eraseModeButton,
        &drawModeButton,
        &shapeModeButton,
        &penTypeChoiceButton,
        &shapeChoiceButton
    };
}

void Application::drawMenu()
{
    ofSetBackgroundColor(255);
    ofSetColor(0);
    for (Button *b : buttons)
    {
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
    grid.draw();

    // Dessiner le curseur personnalisé
    ofSetColor(0);

    // Curseur pour le dessin : Une croix plus grosse
    if (cursorMode == DRAW) 
    {
        int cursorSize = 15;
        int thickness = 2;

        ofSetLineWidth(thickness);
        ofDrawLine(ofGetMouseX() - cursorSize, ofGetMouseY(), ofGetMouseX() + cursorSize, ofGetMouseY());
        ofDrawLine(ofGetMouseX(), ofGetMouseY() - cursorSize, ofGetMouseX(), ofGetMouseY() + cursorSize);
    }

    // Curseur pour l'effacement : Un cercle
    else if (cursorMode == ERASE) 
    {
        int eraserSize = 20; 
        ofNoFill();
        ofDrawCircle(ofGetMouseX(), ofGetMouseY(), eraserSize);
        ofFill();
    }
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
        if (buttonNumber < (int)sizeof(buttons))
        {
            buttons[buttonNumber]->mousePressed(x, y, button);
        }
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

void Application::exportImage()
{
    cursorMode = DEFAULT;
}

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

void Application::fastForward()
{
    cursorMode = DEFAULT;
}

void Application::eraseMode()
{
    ofLog() << "Mode: ERASE";  // Debug
    cursorMode = ERASE;
    ofHideCursor();
}

void Application::drawMode()
{
    ofLog() << "Mode: DRAW";  // Debug
    cursorMode = DRAW;
    ofHideCursor();
}

void Application::shapeMode()
{
    cursorMode = DEFAULT;
    ofShowCursor();
}

void Application::penTypeChoice()
{
    // TODO
    cursorMode = DEFAULT;
}

void Application::shapeChoice()
{
    // TODO
    cursorMode = DEFAULT;
}