#include <algorithm>

#include "ofMain.h"

#ifndef BUTTON
#define BUTTON

class Application; // Forward declaration

class Button
{
public:
    int x;
    int y;
    int w;
    int h;

    Button();
    void setup(int x, int y, int w, int h, Application *application, void (Application::*callback)(), ofImage icon);
    void setIcon(ofImage icon);

    void update();
    void draw();
    void exit();
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseScrolled(int x, int y, float scrollX, float scrollY);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);

private:
    void (Application::*callback)();
    Application *application;
    ofImage icon;
};

#endif