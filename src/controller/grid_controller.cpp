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
    float scaleX = ((float)displayWidth) / grid.w;
    float scaleY = ((float)displayHeight) / grid.h;

    int y = 0;
    for (vector<Cell *> line : grid.grid)
    {
        int x = 0;
        for (Cell *cell : line)
        {
            auto color = cell->getCellColor();
            ofSetColor(color);
            ofDrawRectangle(
                (int)x * scaleX + displayPosX,
                (int)y * scaleY + displayPosY,
                (int)scaleX,
                (int)scaleY);
            x++;
        }
        y++;
    }

    ofSetColor(0, 0, 0, 150);
    ofSetLineWidth(2);

    for (int x = 0; x < grid.w; x++)
    {
        ofDrawLine(x * scaleX + displayPosX, displayPosY, x * scaleX, displayPosY + displayHeight);
    }
    for (int y = 0; y < grid.h; y++)
    {
        ofDrawLine(displayPosX, y * scaleY + displayPosY, displayPosX + displayWidth, y * scaleY + displayPosY);
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
