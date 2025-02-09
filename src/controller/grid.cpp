#include "grid.h"

//--------------------------------------------------------------
void Grid::setup(int x, int y, int w, int h)
{
    displayPosX = x;
    displayPosY = y;
    displayWidth = w;
    displayHeight = h;
}

//--------------------------------------------------------------
void Grid::draw()
{
    ofSetColor(0);
    ofDrawRectangle(displayPosX, displayPosY, displayWidth, displayHeight);
}

//--------------------------------------------------------------
void Grid::exit()
{
}

//--------------------------------------------------------------
void Grid::keyPressed(int key)
{
}

//--------------------------------------------------------------
void Grid::keyReleased(int key)
{
}

//--------------------------------------------------------------
void Grid::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void Grid::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void Grid::mousePressed(int x, int y, int button)
{
}

//--------------------------------------------------------------
void Grid::mouseReleased(int x, int y, int button)
{
}

//--------------------------------------------------------------
void Grid::mouseScrolled(int x, int y, float scrollX, float scrollY)
{
}

//--------------------------------------------------------------
void Grid::mouseEntered(int x, int y)
{
}

//--------------------------------------------------------------
void Grid::mouseExited(int x, int y)
{
}

//--------------------------------------------------------------
void Grid::windowResized(int w, int h)
{
}
