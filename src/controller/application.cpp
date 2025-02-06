#include "application.h"

void someFunction()
{

    ofLog() << "SOME FUNCTION CALLED";
}

//--------------------------------------------------------------
void Application::setup()
{
    ofSetWindowShape(WINDOW_WIDTH, WINDOW_HEIGHT);
    grid.setup(0, MENU_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT);

    buttons = vector<Button>{
        Button(MENU_BUTTON_WIDTH * 0, 0, MENU_BUTTON_WIDTH, MENU_BUTTON_WIDTH, someFunction), // importButton
        Button(MENU_BUTTON_WIDTH * 1, 0, MENU_BUTTON_WIDTH, MENU_BUTTON_WIDTH, someFunction), // exportButton
        Button(MENU_BUTTON_WIDTH * 2, 0, MENU_BUTTON_WIDTH, MENU_BUTTON_WIDTH, someFunction), // playButton
        Button(MENU_BUTTON_WIDTH * 3, 0, MENU_BUTTON_WIDTH, MENU_BUTTON_WIDTH, someFunction), // fastForwardButton
        Button(MENU_BUTTON_WIDTH * 4, 0, MENU_BUTTON_WIDTH, MENU_BUTTON_WIDTH, someFunction), // eraseModeButton
        Button(MENU_BUTTON_WIDTH * 5, 0, MENU_BUTTON_WIDTH, MENU_BUTTON_WIDTH, someFunction), // drawModeButton
        Button(MENU_BUTTON_WIDTH * 6, 0, MENU_BUTTON_WIDTH, MENU_BUTTON_WIDTH, someFunction), // shapeModeButton
        Button(MENU_BUTTON_WIDTH * 7, 0, MENU_BUTTON_WIDTH, MENU_BUTTON_WIDTH, someFunction), // penTypeChoiceButton
        Button(MENU_BUTTON_WIDTH * 8, 0, MENU_BUTTON_WIDTH, MENU_BUTTON_WIDTH, someFunction)  // shapeChoiceButton
    };
}

void Application::drawMenu()
{
    for (Button b : buttons) {
        b.draw();
    }
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
        if (buttonNumber < sizeof(buttons))
        {
            ofLog() << "passing click to callback of button: " << buttonNumber;
            buttons[buttonNumber].callback();
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
