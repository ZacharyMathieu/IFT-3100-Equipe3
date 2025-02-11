#include "grid_controller.h"

//--------------------------------------------------------------
void GridController::setup(int x, int y, int w, int h)
{
    displayPosX = x;
    displayPosY = y;
    displayWidth = w;
    displayHeight = h;
}

//--------------------------------------------------------------
void GridController::draw()
{
    ofSetColor(0, 0, 0, 150);
    ofSetLineWidth(2);

    for (int i = 0; i < displayWidth; i += 20)
    {
        ofDrawLine(displayPosX + i, displayPosY, displayPosX + i, displayPosY + displayHeight);
    }
    for (int j = 0; j < displayHeight; j += 20)
    {
        ofDrawLine(displayPosX, displayPosY + j, displayPosX + displayWidth, displayPosY + j);
    }
}
//--------------------------------------------------------------
void GridController::exit()
{
}

//--------------------------------------------------------------
void GridController::keyPressed(int key)
{
}

//--------------------------------------------------------------
void GridController::keyReleased(int key)
{
}

//--------------------------------------------------------------
void GridController::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void GridController::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void GridController::mousePressed(int x, int y, int button)
{
}

//--------------------------------------------------------------
void GridController::mouseReleased(int x, int y, int button)
{
}

//--------------------------------------------------------------
void GridController::mouseScrolled(int x, int y, float scrollX, float scrollY)
{
}

//--------------------------------------------------------------
void GridController::mouseEntered(int x, int y)
{
}

//--------------------------------------------------------------
void GridController::mouseExited(int x, int y)
{
}

//--------------------------------------------------------------
void GridController::windowResized(int w, int h)
{
}
