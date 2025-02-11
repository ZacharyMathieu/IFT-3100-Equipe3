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

    importImageButton.setup(MENU_BUTTON_WIDTH * 0, 0, MENU_BUTTON_WIDTH, MENU_HEIGHT, this, importImage, importImageIcon);
    exportImageButton.setup(MENU_BUTTON_WIDTH * 1, 0, MENU_BUTTON_WIDTH, MENU_HEIGHT, this, exportImage, exportImageIcon);
    playButton.setup(MENU_BUTTON_WIDTH * 2, 0, MENU_BUTTON_WIDTH, MENU_HEIGHT, this, play, playIcon);
    fastForwardButton.setup(MENU_BUTTON_WIDTH * 3, 0, MENU_BUTTON_WIDTH, MENU_HEIGHT, this, fastForward, fastForwardIcon);
    eraseModeButton.setup(MENU_BUTTON_WIDTH * 4, 0, MENU_BUTTON_WIDTH, MENU_HEIGHT, this, eraseMode, eraseModeIcon);
    drawModeButton.setup(MENU_BUTTON_WIDTH * 5, 0, MENU_BUTTON_WIDTH, MENU_HEIGHT, this, drawMode, drawModeIcon);
    shapeModeButton.setup(MENU_BUTTON_WIDTH * 6, 0, MENU_BUTTON_WIDTH, MENU_HEIGHT, this, shapeMode, shapeModeIcon);
    penTypeChoiceButton.setup(MENU_BUTTON_WIDTH * 7, 0, MENU_BUTTON_WIDTH, MENU_HEIGHT, this, penTypeChoice, penTypeChoiceIcon);
    shapeChoiceButton.setup(MENU_BUTTON_WIDTH * 8, 0, MENU_BUTTON_WIDTH, MENU_HEIGHT, this, shapeChoice, shapeChoiceIcon);
    buttons = vector<Button *>{
        &importImageButton,
        &exportImageButton,
        &playButton,
        &fastForwardButton,
        &eraseModeButton,
        &drawModeButton,
        &shapeModeButton,
        &penTypeChoiceButton,
        &shapeChoiceButton};
}

void Application::drawMenu()
{
    ofSetBackgroundColor(255);
    for (Button *b : buttons)
    {
        b->draw();
    }
    ofSetColor(0);
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
    grid.draw();

    // curseur personnalisé
    ofSetColor(0);
    if (cursorMode == DRAW) 
    {
        ofDrawLine(ofGetMouseX() - 5, ofGetMouseY(), ofGetMouseX() + 5, ofGetMouseY());
        ofDrawLine(ofGetMouseX(), ofGetMouseY() - 5, ofGetMouseX(), ofGetMouseY() + 5);
    } 
    else if (cursorMode == ERASE) 
    {
        ofNoFill();
        ofDrawCircle(ofGetMouseX(), ofGetMouseY(), 10);
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
        int buttonNumber = x / MENU_BUTTON_WIDTH;
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
    // TODO
    cout << "importImage\n";
}

void Application::exportImage()
{
    // TODO
    cout << "exportImage\n";
}

void Application::play()
{
    // TODO
    cout << "play\n";
    isRunning = !isRunning;
    if (isRunning)
    {
        playButton.setIcon(pauseIcon);
    }
    else
    {
        playButton.setIcon(playIcon);
    }
}

void Application::fastForward()
{
    // TODO
    cout << "fastForward\n";
}

void Application::eraseMode() 
{
    cursorMode = ERASE;
    ofHideCursor();
}

void Application::drawMode() 
{
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
    cout << "penTypeChoice\n";
}

void Application::shapeChoice()
{
    // TODO
    cout << "shapeChoice\n";
}
