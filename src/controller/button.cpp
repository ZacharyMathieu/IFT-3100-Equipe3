#include "button.h"

Button::Button(int x, int y, int w, int h, void(*callback)())
{
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->callback = callback;
}

//--------------------------------------------------------------
void Button::update()
{
}

//--------------------------------------------------------------
void Button::draw()
{
}

//--------------------------------------------------------------
void Button::exit()
{
}

//--------------------------------------------------------------
void Button::keyPressed(int key)
{
}

//--------------------------------------------------------------
void Button::keyReleased(int key)
{
}

//--------------------------------------------------------------
void Button::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void Button::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void Button::mousePressed(int x, int y, int button)
{
    ofLog() << "MOUSE PRESSED DETECTED";
}

//--------------------------------------------------------------
void Button::mouseReleased(int x, int y, int button)
{
}

//--------------------------------------------------------------
void Button::mouseScrolled(int x, int y, float scrollX, float scrollY)
{
}

//--------------------------------------------------------------
void Button::mouseEntered(int x, int y)
{
}

//--------------------------------------------------------------
void Button::mouseExited(int x, int y)
{
}

//--------------------------------------------------------------
void Button::windowResized(int w, int h)
{
}