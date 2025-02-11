#include "button.h"

Button::Button() {}

void Button::setup(int x, int y, int w, int h, Application *application, void (Application::*callback)(), ofImage *icon)
{
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->application = application;
    this->callback = callback;
    setIcon(icon);
}

void Button::setIcon(ofImage *icon)
{
    this->icon = icon;
}

//--------------------------------------------------------------
void Button::update()
{
}

//--------------------------------------------------------------
void Button::draw()
{
    if (icon->isAllocated())
    {
        icon->draw(x, y, w, h);
    }
    else
    {
        ofLogError() << "Button icon is not allocated!\n";
    }
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
    (application->*callback)();
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